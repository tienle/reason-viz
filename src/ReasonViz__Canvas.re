open ReasonViz__Types;

module Shape = {
  type t;

  external empty: unit => t = "#null";
  [@bs.send] external set: (t, string, 'a) => unit = "set";
  [@bs.send] external get: (t, string) => string = "get";
  [@bs.send] external _getAttrs: (t, string) => Js.t({..}) = "get";
  [@bs.send] external attr: (t, string, 'a) => unit = "attr";
  [@bs.send] external attrs: (t, Js.t({..})) => unit = "attr";
  [@bs.send] external destroy: t => unit = "destroy";
  [@bs.send] external remove: t => unit = "remove";
  [@bs.send] external getBBox: t => Js.t({..}) = "getBBox";
  [@bs.send] external getTotalLength: t => float = "getTotalLength";
  [@bs.send] external _getPoint: (t, ~ratio: float) => jsPoint = "getPoint";
  let getAttrs = t => _getAttrs(t, "attrs");
  let getPoint = (shape, ~ratio) => {
    _getPoint(shape, ~ratio) |> pointFromJs;
  };

  [@bs.send]
  external _animate:
    (
      t,
      ~attrs: Js.t({..}),
      ~duration: option(int),
      ~easing: option(string),
      ~callback: option(string),
      ~delay: option(int)
    ) =>
    unit =
    "animate";

  let animate =
      (shape, ~attrs, ~duration=?, ~easing=?, ~callback=?, ~delay=?, ()) => {
    _animate(shape, ~attrs, ~duration, ~easing, ~callback, ~delay);
  };

  [@bs.send] external stopAnimate: t => unit = "stopAnimate";
};

module Group = {
  type t;

  [@bs.send] external makeWith: (t, Js.t({..})) => t = "addGroup";
  [@bs.send] external make: t => t = "addGroup";
  [@bs.send] external set: (t, string, 'a) => unit = "set";
  [@bs.send] external clear: t => unit = "clear";
  [@bs.send] external sort: t => unit = "sort";
  [@bs.send] external findShapeById: (t, string) => Shape.t = "findById";
  [@bs.send]
  external addShape: (t, string, Js.t({..})) => Shape.t = "addShape";

  [@bs.send] external resetMatrix: t => unit = "resetMatrix";
  [@bs.send] external translate: (t, float, float) => unit = "translate";
  [@bs.send] external getMatrix: t => matrix = "getMatrix";
  [@bs.send]
  external findAllShapes: (t, Shape.t => bool) => array(Shape.t) = "findAll";
};

module PathUtil = {
  [@bs.module "./external/util/path.js"]
  external catmullRomToBezier: array(float) => array(Path.t) =
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
[@bs.send] external getEl: (t, string) => element = "get";
[@bs.send] external getInt: (t, string) => int = "get";
[@bs.send] external on: (t, string, 'a => unit) => unit = "on";
[@bs.send] external off: (t, string, 'a => unit) => unit = "off";

/* [@bs.send] */
/* external getElementById: (document, string) => Dom.element = "getElementById"; */

let create = (~containerId, ~width, ~height, ~renderer, ~pixelRatio=?, ()) => {
  createCanvas(
    options(~containerId, ~width, ~height, ~renderer, ~pixelRatio?, ()),
  );
};
