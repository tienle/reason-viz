open ReasonViz__Types;

module Shape = {
  type t;

  [@bs.send] external set: (t, string, 'a) => unit = "set";
  external empty: unit => t = "#null";
  [@bs.send] external getBBox: t => Js.t({..}) = "getBBox";
};

module Group = {
  type t;

  [@bs.send] external makeWith: (t, Js.t({..})) => t = "addGroup";
  [@bs.send] external make: t => t = "addGroup";
  [@bs.send] external set: (t, string, 'a) => unit = "set";
  [@bs.send] external clear: t => unit = "clear";
  [@bs.send]
  external addShape: (t, string, Js.t({..})) => Shape.t = "addShape";

  [@bs.send] external resetMatrix: t => unit = "resetMatrix";
  [@bs.send] external translate: (t, int, int) => unit = "translate";
  [@bs.send] external getMatrix: t => matrix = "getMatrix";
};

module PathUtil = {
  [@bs.module "./external/util/path.js"]
  external catmullRomToBezier: array(int) => array(Path.t) =
    "catmullRomToBezier";
};

type t;

[@bs.deriving abstract]
type options = {
  containerId: string,
  width: int,
  height: int,
  renderer: string,
  [@bs.optional]
  pixelRatio: int,
};

[@bs.send] external draw: t => unit = "draw";
[@bs.send] external clear: t => unit = "clear";

[@bs.new] [@bs.module "@antv/g/lib"]
external createCanvas: options => t = "Canvas";

[@bs.send] external addGroup: (t, Js.t({..})) => Group.t = "addGroup";

/* [@bs.send] */
/* external getElementById: (document, string) => Dom.element = "getElementById"; */

let create = (~containerId, ~width, ~height, ~renderer, ~pixelRatio=?, ()) => {
  createCanvas(
    options(~containerId, ~width, ~height, ~renderer, ~pixelRatio?, ()),
  );
};