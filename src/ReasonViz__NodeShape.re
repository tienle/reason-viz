open ReasonViz__Types;

module RN = ReasonViz__Node;
module Canvas = ReasonViz__Canvas;

module type Shape = {let draw: RN.t => Canvas.Shape.t;};

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
           let getShapeStyle: RN.Model.t => Js.Dict.t(string);
         },
       )
       : Shape => {
  let drawShape = (node: RN.t) => {
    let style = Shape.getShapeStyle(node.model);
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
    let shape = drawShape(node);
    let _ = Belt.Option.flatMap(node.model.label, drawLabel(node));

    shape;
  };
};

module Circle =
  Make({
    let shapeType = "circle";

    let getShapeStyle = (model: RN.Model.t) => {
      let (size, _) = model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let styles =
        Js.Dict.fromList([
          ("x", "0"),
          ("y", "0"),
          ("r", string_of_int(size / 2)),
        ]);

      [RN.Model.default^.styles, styles, model.styles]
      |> ReasonViz__Utils.mergeDict;
    };
  });

module Rect =
  Make({
    let shapeType = "rect";

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let x = 0 - width / 2;
      let y = 0 - height / 2;
      let styles =
        Js.Dict.fromList([
          ("x", string_of_int(x)),
          ("y", string_of_int(y)),
          ("width", string_of_int(width)),
          ("height", string_of_int(height)),
        ]);

      [RN.Model.default^.styles, styles, model.styles]
      |> ReasonViz__Utils.mergeDict;
    };
  });

module Ellipse =
  Make({
    let shapeType = "ellipse";

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let rx = width / 2;
      let ry = height / 2;
      let styles =
        Js.Dict.fromList([
          ("x", "0"),
          ("y", "0"),
          ("rx", string_of_int(rx)),
          ("ry", string_of_int(ry)),
        ]);

      [RN.Model.default^.styles, styles, model.styles]
      |> ReasonViz__Utils.mergeDict;
    };
  });

module Image =
  Make({
    let shapeType = "image";

    let getShapeStyle = (model: RN.Model.t) => {
      let (width, height) =
        model.props.getExn("size") |> RN.ShapeValue.toPairInt;
      let x = 0 - width / 2;
      let y = 0 - height / 2;
      let img = model.props.getExn("img") |> RN.ShapeValue.toString;

      let styles =
        Js.Dict.fromList([
          ("x", string_of_int(x)),
          ("y", string_of_int(y)),
          ("width", string_of_int(width)),
          ("height", string_of_int(height)),
          ("img", img),
        ]);

      [RN.Model.default^.styles, styles, model.styles]
      |> ReasonViz__Utils.mergeDict;
    };
  });
