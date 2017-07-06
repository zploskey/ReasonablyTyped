type t =
  | Class
  | Variable string
  | None
  | NotFound;

let rec get key table => {
  let lookup =
    try (List.assoc key table) {
    | Not_found => NotFound
    };
  switch lookup {
  | Variable s => get s table
  | match => match
  }
};

let create statements =>
  List.map
    Modulegen.BsDecl.(
      fun
      | VarDecl id type_of => (id, None)
      | ClassDecl id type_of => (id, Class)
      | _ => ("", None)
    )
    statements |>
  List.filter (fun (key, _) => key != "");

let show table => {
  print_endline "Types:";
  List.iter
    (
      fun (id, typeof) =>
        print_endline (
          "| " ^
          id ^
          " => " ^ (
            switch typeof {
            | Class => "Class"
            | None => "None"
            | NotFound => "NotFound"
            }
          )
        )
    )
    table;
  print_newline ()
};
