let format_obj_key key =>
  if (String.contains key '-') {
    "'" ^ key ^ "'"
  } else {
    key
  };

let rec show_type =
  Modulegen.(
    fun
    | BsType.Regex => "RegExp"
    | BsType.Optional t => show_type t ^ "?"
    | BsType.Any => "any"
    | BsType.AnyObject => "Object"
    | BsType.AnyFunction => "Function"
    | BsType.Unit => "void"
    | BsType.Dict t => "{ [key: string]: " ^ show_type t ^ " }"
    | BsType.Tuple types => "[" ^ (List.map show_type types |> String.concat ", ") ^ "]"
    | BsType.Array t => show_type t ^ "[]"
    | BsType.Typeof t => "typeof " ^ show_type t
    | BsType.Function type_params params return =>
      (List.length type_params > 0 ? "<" : "") ^
      String.concat ", " type_params ^
      (List.length type_params > 0 ? ">" : "") ^
      "(" ^
      String.concat
        ", "
        (
          List.map
            (
              fun (name, type_of) =>
                switch type_of {
                | BsType.Unit => ""
                | BsType.Optional t => name ^ "?: " ^ show_type t
                | _ => name ^ ": " ^ show_type type_of
                }
            )
            params
        ) ^
      "): " ^ show_type return
    | BsType.Null => "null"
    | BsType.Number => "number"
    | BsType.Boolean => "boolean"
    | BsType.String => "string"
    | BsType.Union types => String.concat " | " (List.map show_type types)
    | BsType.Object props =>
      "{ " ^
      String.concat
        ", "
        (
          List.map
            (
              fun (key, prop) =>
                if (key == "$$callProperty") {
                  show_type prop
                } else {
                  format_obj_key key ^ ": " ^ show_type prop
                }
            )
            props
        ) ^ " }"
    | BsType.Class props =>
      "{ " ^
      String.concat "; " (List.map (fun (key, prop) => key ^ ": " ^ show_type prop) props) ^ " }"
    | BsType.Named s => s
    | BsType.StringLiteral t => "\"" ^ t ^ "\""
  );

let rec show_decl =
  Modulegen.(
    fun
    | BsDecl.ExportsDecl of_type => "declare module.exports: " ^ show_type of_type
    | BsDecl.ModuleDecl name decls =>
      "declare module " ^ name ^ " {\n  " ^ String.concat "\n  " (List.map show_decl decls) ^ "\n}"
    | BsDecl.TypeDecl id of_type => "declare type " ^ id ^ " = " ^ show_type of_type
    | BsDecl.FuncDecl name of_type => "declare export function " ^ name ^ show_type of_type
    | BsDecl.VarDecl name of_type => "declare export var " ^ name ^ ": " ^ show_type of_type
    | BsDecl.ClassDecl name of_type => "declare class " ^ name ^ " " ^ show_type of_type
    | BsDecl.InterfaceDecl name of_type => "declare interface " ^ name ^ " " ^ show_type of_type
  );