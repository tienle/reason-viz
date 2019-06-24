open ReasonViz__Types;

module RN = ReasonViz__Node;
module Canvas = ReasonViz__Canvas;
module Util = ReasonViz__Utils;

module type Shape = {
  let draw: RN.t => unit;

  let getAnchorPoints: RN.Model.t => list((float, float));
};

type shapes = Js.Dict.t(module Shape);

let shapes = Js.Dict.empty();

let register: (string, (module Shape)) => unit =
  (name, shape) => {
    Js.Dict.set(shapes, name, shape);
  };

let get = Js.Dict.unsafeGet(shapes);

module Make =
       (
         Shape: {
           let shapeType: string;
           let getAnchorPoints: RN.Model.t => list((float, float));
           let getShapeStyle: RN.Model.t => Js.t({..});
         },
       )
       : Shape => {
  let getAnchorPoints = Shape.getAnchorPoints;

  let drawShape = (node: RN.t) => {
    let style =
      [
        Util.castToJsObj(RN.Model.default^.styles),
        Shape.getShapeStyle(node.model),
        Util.castToJsObj(node.model.styles),
      ]
      |> Util.mergeJsObjects;

    let shape =
      Canvas.Group.addShape(node.group, Shape.shapeType, {"attrs": style});
    Canvas.Shape.set(shape, "className", Shape.shapeType ++ "shape");
    shape;
  };

  let getLabelStyleByPosition = (model: RN.Model.t, label: RN.Label.t) => {
    let (width, height) =
      model.props.getExn("size") |> RN.ShapeValue.toPairInt;

    let offset =
      switch (label.cfg) {
      | Some(cfg) => cfg.offset
      | None => 0
      };
    let position =
      switch (label.cfg) {
      | Some(cfg) => cfg.position
      | None => `center
      };

    let textAttr = ("text", label.text);

    let styles =
      switch (position) {
      | `center => [textAttr, ("x", "0"), ("y", "0")]
      | `top => [
          textAttr,
          ("x", "0"),
          ("y", string_of_int(0 - height / 2 - offset)),
          ("textBaseline", "bottom"),
        ]
      | `bottom => [
          textAttr,
          ("x", "0"),
          ("y", string_of_int(height / 2 + offset)),
          ("textBaseline", "top"),
        ]
      | `left => [
          textAttr,
          ("x", string_of_int(0 - width / 2 - offset)),
          ("y", "0"),
          ("textAlign", "right"),
        ]
      | `right => [
          textAttr,
          ("x", string_of_int(width / 2 + offset)),
          ("y", "0"),
          ("textAlign", "left"),
        ]
      };

    styles |> Js.Dict.fromList;
  };

  let getLabelStyle = (node: RN.t, label) => {
    let styles = getLabelStyleByPosition(node.model, label);

    let labelStyles =
      switch (label.cfg) {
      | Some(cfg) => cfg.styles
      | None => StylesList.make([])
      };

    [RN.Label.default^.styles, styles, labelStyles]
    |> ReasonViz__Utils.mergeDict;
  };

  let drawLabel = (node, label) => {
    let style = getLabelStyle(node, label);
    let labelShape =
      Canvas.Group.addShape(node.group, "text", {"attrs": style});
    Canvas.Shape.set(labelShape, "className", Shape.shapeType ++ "label");
    Some(labelShape);
  };

  let translateToPosition = (node: RN.t) => {
    Canvas.Group.resetMatrix(node.group);
    Canvas.Group.translate(node.group, node.x, node.y);
  };

  let draw = (node: RN.t) => {
    Canvas.Group.clear(node.group);
    translateToPosition(node);
    node.shape = drawShape(node);
    let _ = Belt.Option.flatMap(node.model.label, drawLabel(node));

    ();
  };
};

module Circle =
  Make({
    let shapeType = "circle";

    let getAnchorPoints = (model: RN.Model.t) => model.anchorPoints;

    let getShapeStyle = (model: RN.Model.t) => {
      let (size, _) = model.props.getExn("size") |> RN.ShapeValue.toPairInt;

      Util.makeObj({"x": 0, "y": 0, "r": size / 2});
    };
  });

module Rect =
  Make({
    let shapeType = "rect";

    let getAnchorPoints = (model: RN.Model.t) => model.anchorPoints;

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let x = 0 - width / 2;
      let y = 0 - height / 2;

      Util.makeObj({"x": x, "y": y, "width": width, "height": height});
    };
  });

module Ellipse =
  Make({
    let shapeType = "ellipse";

    let getAnchorPoints = (model: RN.Model.t) => model.anchorPoints;

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let rx = width / 2;
      let ry = height / 2;
      Util.makeObj({"x": 0, "y": 0, "rx": rx, "ry": ry});
    };
  });

module Image =
  Make({
    let shapeType = "image";

    let getAnchorPoints = (model: RN.Model.t) => model.anchorPoints;

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let x = 0 - width / 2;
      let y = 0 - height / 2;
      let img = model.props.getExn("img") |> RN.ShapeValue.toString;

      Util.makeObj({
        "x": x,
        "y": y,
        "width": width,
        "height": height,
        "img": img,
      });
    };
  });
