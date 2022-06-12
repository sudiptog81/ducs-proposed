:- dynamic(visited/2).
:- dynamic(state/2).

% 1. (x, y) if x < 4 -> (4, y)
state(X, Y) :- X < 4,
    not(visited(4, Y)),
    assertz(visited(X, Y)),
    writef("Fill the 4 gal jug.\n\t(%w, %w) --> (4, %w)\n", [X, Y, Y]),
    nl, state(4, Y).

% 2. (x, y) if y < 3 -> (x, 3)
state(X, Y) :- Y < 3,
    not(visited(X, 3)),
    assertz(visited(X, Y)),
    writef("Fill the 3 gal jug.\n\t(%w, %w) --> (%w, 3)\n", [X, Y, X]),
    nl, state(X, 3).

% 5. (x, y) if x > 0 -> (0, y)
state(X, Y) :- X > 0,
    not(visited(0, Y)),
    assertz(visited(X, Y)),
    writef("Empty the 4 gal jug on ground.\n\t(%w, %w) --> (0, %w)\n", [X, Y, Y]),
    nl, state(0, Y).

% 6. (x, y) if y > 0 -> (x, 0)
state(X, Y) :- Y > 0,
    not(visited(X, 0)),
    assertz(visited(X, 0)),
    writef("Empty the 3 gal jug on ground.\n\t(%w, %w) --> (%w, 0)\n", [X, Y, X]),
    nl, state(X, 0).

% 7. (x, y) if x + y >= 4 and y > 0 -> (4, y - (4 - x))
state(X, Y) :- X + Y >= 4, Y > 0,
    Y1 is Y - (4 - X),
    not(visited(4, Y1)),
    assertz(visited(X, Y)),
    writef("Pour water from 3 gal jug to 4 gal until it is full.\n\t(%w, %w) --> (0, %w)\n", [X, Y, Y1]),
    nl, state(4, Y1).

% 8. (x, y) if x + y >= 3 and x > 0 -> (x - (3 - y), 3)
state(X, Y) :- X + Y >= 3, X > 0,
    X1 is X - (3 - Y),
    not(visited(X1, 3)),
    assertz(visited(X, Y)),
    writef("Pour water from 4 gal jug to 3 gal until it is full.\n\t(%w, %w) --> (%w, 3)\n", [X, Y, X1]),
    nl, state(X1 ,3).

% 9. (x, y) if x + y <= 4 and y > 0 -> (x + y, 0)
state(X, Y) :- 4 >= X + Y, Y > 0,
    X1 is X + Y,
    not(visited(X1, 0)),
    assertz(visited(X, Y)),
    writef("Pour all the water from 3 gal jug to 4 gal.\n\t(%w, %w) --> (%w, 0)\n", [X, Y, X1]),
    nl, state(X1, 0).

% 10. (x, y) if x + y <= 3 and x > 0 -> (0, y + x)
state(X, Y) :- 3 >= X + Y, X > 0,
    Y1 is X + Y,
    not(visited(0, Y1)),
    assertz(visited(X, Y)),
    writef("Pour all the water from 4 gal jug to 3 gal.\n\t(%w, %w) --> (0, %w)\n", [X, Y, Y1]),
    nl, state(0, Y1).

% 11. (0, 2) -> (2, 0)
state(0, 2) :- not(visited(2, 0)),
    assertz(visited(0, 2)),
    writef("Pour 2 gallons from 3 gal jug to 4 gal.\n\t(0, 2) -> (2, 0)\n"),
    nl, state(2,0).

% 12. (2, y) -> (0, y)
state(2, Y) :- not(visited(0, Y)),
    assertz(visited(2, Y)),
    writef("Empty 2 gallons from 4 gal jug on the ground.\n\t(2, %w) -> (0, %w)\n", [Y]),
    nl, state(0, Y).

% driver code
input :-
  write("Enter final desired volume in 4 gal jug: "),
  read(X),
  write("Enter final desired volume in 3 gal jug: "),
  read(Y),
  nl, asserta(state(X, Y)).

main :-
  input,
  state(0, 0),
  halt.
