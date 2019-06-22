/* const node = new Node({ */
/*   id: '',            */
/*   shape: '', */
/*   x: 0, */
/*   y: 0, */
/*   size: [], */
/*   style: {...}, */
/*   label: 'label text', */
/*   labelCfg: { */
/*     position: 'top', */
/*     style: {...} */
/*   }, */
/*   anchorPoints: [] */
/*  }); */

type matrix;

exception NodeNotFound(string);

[@bs.deriving jsConverter]
type point = {
  x: int,
  y: int,
};

type size = {
  width: int,
  height: int,
};

module Path = {
  type t = (string, array(int));
};

module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = String.length;
  });

module StylesList = {
  type t = Js.Dict.t(string);

  let make = ReasonViz__Drawing_Styles.make;
};