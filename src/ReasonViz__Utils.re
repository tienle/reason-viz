module UniqueId = {
  module type Salt = {let prefix: string;};

  module Make = (M: Salt) => {
    let current = ref(0);

    let getUniqueId = () => {
      let ret = current^;
      incr(current);
      M.prefix ++ string_of_int(ret);
    };
  };
};

let mergeDict = dicts => {
  dicts |> List.map(Js.Dict.entries) |> Array.concat |> Js.Dict.fromArray;
};
