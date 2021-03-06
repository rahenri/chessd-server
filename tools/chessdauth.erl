%%%----------------------------------------------------------------------
%%% File    : extauth.erl
%%% Author  : Leif Johansson <leifj@it.su.se>
%%% Purpose : External authentication using a simple port-driver
%%% Created : 30 Jul 2004 by Leif Johansson <leifj@it.su.se>
%%%
%%%
%%% ejabberd, Copyright (C) 2002-2008   Process-one
%%% ejabberd, Copyright (C) 2002-2008   Process-one
%%%
%%% This program is free software; you can redistribute it and/or
%%% modify it under the terms of the GNU General Public License as
%%% published by the Free Software Foundation; either version 2 of the
%%% License, or (at your option) any later version.
%%%
%%% This program is distributed in the hope that it will be useful,
%%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
%%% General Public License for more details.
%%%                         
%%% You should have received a copy of the GNU General Public License
%%% along with this program; if not, write to the Free Software
%%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
%%% 02111-1307 USA
%%%
%%%----------------------------------------------------------------------

-module(chessdauth).
-author('leifj@it.su.se').

-export([start/2, stop/1, init/2,
	 check_password/3, set_password/3, is_user_exists/2, register_user/3]).


start(Host, ExtPrg) ->
    spawn(?MODULE, init, [Host, ExtPrg]).

init(Host, ExtPrg) ->
    register(gen_mod:get_module_proc(Host, eauth), self()),
    process_flag(trap_exit,true),
    Port = open_port({spawn, ExtPrg}, [{packet,2}]),
    loop(Port).

stop(Host) ->
    gen_mod:get_module_proc(Host, eauth) ! stop.

check_password(User, Server, Password) ->
    call_port(Server, ["auth", User, Server, Password]).

is_user_exists(User, Server) ->
    call_port(Server, ["isuser", User, Server]).

set_password(User, Server, Password) ->
    call_port(Server, ["setpass", User, Server, Password]).

register_user(User, Server, Password) ->
    case call_port(Server, ["register", User, Server, Password]) of
    true ->
        {atomic, ok};
    false ->
        call_port(Server, ["fail", User, Server, Password]),
        {error, not_allowed}
    end.

call_port(Server, Msg) ->
    LServer = jlib:nameprep(Server),
    gen_mod:get_module_proc(LServer, eauth) ! {call, self(), Msg},
    receive
	{eauth,Result} ->
	    Result
    end.

loop(Port) ->
    receive
	{call, Caller, Msg} ->
	    Port ! {self(), {command, encode(Msg)}},
	    receive
		{Port, {data, Data}} ->
		    Caller ! {eauth, decode(Data)}
	    end,
	    loop(Port);
	stop ->
	    Port ! {self(), close},
	    receive
		{Port, closed} ->
		    exit(normal)
	    end;
	{'EXIT', Port, Reason} ->
	    io:format("~p ~n", [Reason]),
	    exit(port_terminated)
    end.

join(List, Sep) ->
    lists:foldl(fun(A, "") -> A;
		   (A, Acc) -> Acc ++ Sep ++ A
		end, "", List).

encode(L) ->
    join(L,":").

decode([0,0]) ->
    false;
decode([0,1]) ->
    true.

