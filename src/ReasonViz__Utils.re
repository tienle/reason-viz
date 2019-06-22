open ReasonViz__Types;
module Canvas = ReasonViz__Canvas;

module UniqueId = {
  module type Salt = {let prefix: string;};

  module Make = (M: Salt) => {
    let current = ref(0);

    let generate = () => {
      let ret = current^;
      incr(current);
      M.prefix ++ string_of_int(ret);
    };
  };
};

let mergeDict = dicts => {
  dicts |> List.map(Js.Dict.entries) |> Array.concat |> Js.Dict.fromArray;
};

let makeObj = obj => Js.Obj.assign(Js.Obj.empty(), obj);
let mergeJsObjects = objects => {
  let result = Js.Obj.empty();
  List.fold_left((acc, o) => Js.Obj.assign(acc, o), result, objects);
};
external castToJsObj: Js.Dict.t(string) => Js.t({..}) = "%identity";

let memoize = (~id, ~f) => {
  let table = Belt.HashMap.String.make(~hintSize=100);
  x =>
    switch (Belt.HashMap.String.get(table, id(x))) {
    | Some(y) => y
    | None =>
      let y = f(x);
      Belt.HashMap.String.set(table, id(x), y);
      y;
    };
};

module Math = {
  type jsPoint = {
    .
    "x": int,
    "y": int,
  };

  [@bs.module "./external/util/math.js"]
  external _getCircleIntersectByPoint:
    (Js.t({..}), jsPoint) =>
    Js.Nullable.t({
      ..
      "x": int,
      "y": int,
    }) =
    "getCircleIntersectByPoint";

  let getCircleIntersectByPoint = (circle, point) => {
    (
      _getCircleIntersectByPoint(circle, pointToJs(point))
      |> Js.Nullable.toOption
    )
    ->Belt.Option.map(pointFromJs);
  };

  [@bs.module "./external/util/math.js"]
  external _getEllispeIntersectByPoint:
    (Js.t({..}), jsPoint) =>
    Js.Nullable.t({
      ..
      "x": int,
      "y": int,
    }) =
    "getEllispeIntersectByPoint";

  let getEllispeIntersectByPoint = (ellipse, point) => {
    (
      _getEllispeIntersectByPoint(ellipse, pointToJs(point))
      |> Js.Nullable.toOption
    )
    ->Belt.Option.map(pointFromJs);
  };

  [@bs.module "./external/util/math.js"]
  external _getRectIntersectByPoint:
    (Js.t({..}), jsPoint) =>
    Js.Nullable.t({
      ..
      "x": int,
      "y": int,
    }) =
    "getRectIntersectByPoint";

  let getRectIntersectByPoint = (bbox, point) => {
    (
      _getRectIntersectByPoint(bbox, pointToJs(point)) |> Js.Nullable.toOption
    )
    ->Belt.Option.map(pointFromJs);
  };

  [@bs.module "./external/util/math.js"]
  external _getControlPoint: (jsPoint, jsPoint, float, int) => jsPoint =
    "getControlPoint";

  let getControlPoint = (~start, ~end_, ~curvePosition, ~curveOffset) => {
    _getControlPoint(
      pointToJs(start),
      pointToJs(end_),
      curvePosition,
      curveOffset,
    )
    |> pointFromJs;
  };

  module Vec3 = {
    type t = array(int);

    [@bs.module "@antv/util/lib/matrix/vec3"]
    external transformMat3: (t, t, matrix) => unit = "transformMat3";
  };

  let applyMatrix = (point, matrix, ~tag=1, ()) => {
    let vector = [|point.x, point.y, tag|];

    Vec3.transformMat3(vector, vector, matrix);

    {x: vector[0], y: vector[1]};
  };
};

module Graphic = {
  type bbox = {
    minX: int,
    minY: int,
    maxX: int,
    maxY: int,
    x: int,
    y: int,
    width: int,
    height: int,
    centerX: int,
    centerY: int,
  };

  // TODO: Memoize

  let getBBox = (~shape, ~group) => {
    let bbox = Canvas.Shape.getBBox(shape);

    let leftTop: point = {x: bbox##minX, y: bbox##minY};

    let rightBottom: point = {x: bbox##maxX, y: bbox##maxY};

    let (leftTop, rightBottom) =
      switch (group) {
      | Some(parent) =>
        let matrix = Canvas.Group.getMatrix(parent);
        (
          Math.applyMatrix(leftTop, matrix, ()),
          Math.applyMatrix(rightBottom, matrix, ()),
        );
      | None => (leftTop, rightBottom)
      };

    let minX = leftTop.x;
    let minY = leftTop.y;
    let maxX = rightBottom.x;
    let maxY = rightBottom.y;
    let x = minX;
    let y = minY;
    let width = maxX - minX;
    let height = maxY - minY;
    let centerX = (minX + maxX) / 2;
    let centerY = (minY + maxY) / 2;

    {minX, minY, maxX, maxY, x, y, width, height, centerX, centerY};
  };

  [@bs.module "./external/util/graphic.js"]
  external getLabelPosition:
    (
      ~shape: Canvas.Shape.t,
      ~pointPercent: float,
      ~refX: int,
      ~refY: int,
      ~autoRotate: bool
    ) =>
    {
      ..
      "x": int,
      "y": int,
      "angle": float,
      "rotate": float,
    } =
    "getLabelPosition";

  [@bs.module "./external/util/graphic.js"]
  external getTextAlign: (~labelPosition: string, ~angle: float) => string =
    "getTextAlign";
};
