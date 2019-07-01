open ReasonViz__Types;
open ReasonViz__GraphTypes;

module Canvas = ReasonViz__Canvas;
module Util = ReasonViz__Utils;

module type Shape = {
  let shapeType: string;
  let getAnchorPoints: NodeModel.t => list((float, float));
  let getShapeStyle: NodeModel.t => Js.t({..});
  let afterDraw: node => unit;
  let draw: node => unit;
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
           let getAnchorPoints: NodeModel.t => list((float, float));
           let getShapeStyle: NodeModel.t => Js.t({..});
           let afterDraw: node => unit;
         },
       )
       : Shape => {
  let shapeType = Shape.shapeType;
  let getAnchorPoints = Shape.getAnchorPoints;
  let getShapeStyle = Shape.getShapeStyle;
  let afterDraw = Shape.afterDraw;

  let drawShape = (node: node) => {
    let style =
      [
        Util.castToJsObj(ReasonViz__Node.Model.default^.styles),
        getShapeStyle(node.model),
        Util.castToJsObj(node.model.styles),
      ]
      |> Util.mergeJsObjects;

    let shape =
      Canvas.Group.addShape(node.group, shapeType, {"attrs": style});
    Canvas.Shape.set(shape, "className", shapeType ++ "shape");
    shape;
  };

  let getLabelStyleByPosition = (model: NodeModel.t, label: NodeLabel.t) => {
    let {width, height} = model.size;

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

  let getLabelStyle = (node: node, label) => {
    let styles = getLabelStyleByPosition(node.model, label);

    let labelStyles =
      switch (label.cfg) {
      | Some(cfg) => cfg.styles
      | None => StylesList.make([])
      };

    [ReasonViz__Node.Label.default^.styles, styles, labelStyles]
    |> ReasonViz__Utils.mergeDict;
  };

  let drawLabel = (node, label) => {
    let style = getLabelStyle(node, label);
    let labelShape =
      Canvas.Group.addShape(node.group, "text", {"attrs": style});
    Canvas.Shape.set(labelShape, "className", shapeType ++ "label");
    Some(labelShape);
  };

  let translateToPosition = (node: node) => {
    Canvas.Group.resetMatrix(node.group);
    Canvas.Group.translate(node.group, node.x, node.y);
  };

  let draw = (node: node) => {
    Canvas.Group.clear(node.group);
    translateToPosition(node);
    node.shape = drawShape(node);
    let _ = Belt.Option.flatMap(node.model.label, drawLabel(node));

    afterDraw(node);
    ();
  };
};

module DefaultShape = {
  let getAnchorPoints = (model: NodeModel.t) => model.anchorPoints;
  let afterDraw = _ => ();
};

module Circle =
  Make({
    let shapeType = "circle";

    include DefaultShape;

    let getShapeStyle = (model: NodeModel.t) => {
      let {width, _} = model.size;

      Util.makeObj({"x": 0, "y": 0, "r": width / 2});
    };
  });

module Rect =
  Make({
    let shapeType = "rect";

    include DefaultShape;

    let getShapeStyle = (model: NodeModel.t) => {
      let {width, height} = model.size;
      let x = 0 - width / 2;
      let y = 0 - height / 2;

      Util.makeObj({"x": x, "y": y, "width": width, "height": height});
    };
  });

module Ellipse =
  Make({
    let shapeType = "ellipse";

    include DefaultShape;

    let getShapeStyle = (model: NodeModel.t) => {
      let {width, height} = model.size;
      let rx = width / 2;
      let ry = height / 2;
      Util.makeObj({"x": 0, "y": 0, "rx": rx, "ry": ry});
    };
  });

module Image =
  Make({
    let shapeType = "image";

    include DefaultShape;

    let getShapeStyle = (model: NodeModel.t) => {
      let {width, height} = model.size;
      let x = 0 - width / 2;
      let y = 0 - height / 2;
      let img = model.props.getExn("img") |> ShapeValue.toString;

      Util.makeObj({
        "x": x,
        "y": y,
        "width": width,
        "height": height,
        "img": img,
      });
    };
  });

register("circle", (module Circle));
register("rect", (module Rect));
register("ellipse", (module Ellipse));
register("image", (module Image));
