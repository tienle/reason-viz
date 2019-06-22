open ReasonViz__Types;

module RN = ReasonViz__Node;
module RE = ReasonViz__Edge;
module Util = ReasonViz__Utils;
module Canvas = ReasonViz__Canvas;

module type Shape = {let draw: RE.t => unit;};

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
           let getControlPoints: RE.Model.t => array(point);
           let getPath: array(point) => string;
         },
       )
       : Shape => {
  // TODO: memoize getControlPoints
  let getShapeStyle = (model: RE.Model.t) => {
    //TODO: Shape.getPathPoints()
    let sourcePointArr = [|model.sourcePoint|];
    let targetPointArr = [|model.targetPoint|];
    let controlPoints = Shape.getControlPoints(model);
    let points =
      Array.concat([sourcePointArr, controlPoints, targetPointArr]);

    let path = Shape.getPath(points);

    let styles =
      Js.Dict.fromList([
        ("lineWidth", string_of_int(model.size)),
        ("path", path),
      ]);

    [RE.Model.default^.styles, styles, model.styles]
    |> ReasonViz__Utils.mergeDict;
  };

  let drawShape = (edge: RE.t) => {
    let style = getShapeStyle(edge.model);
    let shape =
      Canvas.Group.addShape(
        edge.group,
        "path",
        {"attrs": style, "className": "edge-shape"},
      );
    shape;
  };

  let getPointOrCallNodeWith = (vertex: RE.Model.vertex, ~f): point => {
    switch (vertex) {
    | `Point(point) => point
    | `Node(node) => f(node)
    | _ => {x: 0, y: 0}
    };
  };

  let getLinkPointForNode = (node: RN.t, point) => {
    let shape = node.shape;
    let bbox =
      Util.Graphic.getBBox(~shape=node.shape, ~group=Some(node.group));
    let centerX = bbox.centerX;
    let centerY = bbox.centerY;
    // TODO: let anchorPoints = getAnchorPoints();
    let intersectPoint =
      switch (node.model.shape) {
      | "circle" =>
        Util.Math.getCircleIntersectByPoint(
          {"x": centerX, "y": centerY, "r": bbox.width / 2},
          point,
        )
      | "ellipse" =>
        Util.Math.getEllispeIntersectByPoint(
          {
            "x": centerX,
            "y": centerY,
            "rx": bbox.width / 2,
            "ry": bbox.height / 2,
          },
          point,
        )
      | _ =>
        Util.Math.getRectIntersectByPoint(
          {
            "x": bbox.x,
            "y": bbox.y,
            "width": bbox.width,
            "height": bbox.height,
          },
          point,
        )
      };

    // TODO: Anchor points

    Belt.Option.getWithDefault(intersectPoint, {x: centerX, y: centerY});
  };

  let getLinkPoint = (name, model: RE.Model.t, controlPoint): point => {
    let (vertex, oppositeVertex) =
      switch (name) {
      | `source => (model.source, model.target)
      | `target => (model.target, model.source)
      };

    switch (vertex) {
    | `Point(point) => point
    | `Node(node) =>
      let prePoint =
        Belt.Option.getWithDefault(
          controlPoint,
          getPointOrCallNodeWith(oppositeVertex, ~f=node =>
            {x: node.x, y: node.y}
          ),
        );
      //TODO: anchorPoint
      getLinkPointForNode(node, prePoint);
    | _ => {x: 0, y: 0}
    };
  };

  let interpolateVertexPoints = (edge: RE.t) => {
    let model = edge.model;

    let getCenterFromNode =
      getPointOrCallNodeWith(~f=node => {
        let bbox =
          Util.Graphic.getBBox(~shape=node.shape, ~group=Some(node.group));
        {x: bbox.centerX, y: bbox.centerY};
      });

    if (model.linkCenter) {
      model.sourcePoint = getCenterFromNode(model.source);
      model.targetPoint = getCenterFromNode(model.target);
    } else {
      let controlPoints =
        switch (model.controlPoints) {
        | [||] => Shape.getControlPoints(model)
        | points => points
        };
      model.sourcePoint =
        getLinkPoint(`source, model, Belt.Array.get(controlPoints, 0));
      model.targetPoint =
        getLinkPoint(
          `target,
          model,
          Belt.Array.get(
            controlPoints,
            Belt.Array.length(controlPoints) - 1,
          ),
        );
    };
  };

  let getLabelStyleByPosition = (edge: RE.t, label: RE.Label.t) => {
    let position =
      switch (label.cfg) {
      | Some(cfg) => cfg.position
      | None => `middle
      };

    let pointPercent =
      switch (position) {
      | `start => 0.0
      | `middle | `center => 0.5
      | `end_ => 1.0
      };

    let labelCfg =
      switch (label.cfg) {
      | Some(cfg) => cfg
      | none => RE.Label.default^
      };

    let {RE.Label.refX, RE.Label.refY} = labelCfg;
    let {RE.Model.sourcePoint, RE.Model.targetPoint} = edge.model;
    let styles =
      if (sourcePoint.x == targetPoint.x && sourcePoint.y == targetPoint.y) {
        [
          ("x", string_of_int(sourcePoint.x + refX)),
          ("y", string_of_int(targetPoint.y + refY)),
        ];
      } else {
        let offsetStyle =
          Util.Graphic.getLabelPosition(
            ~shape=edge.shape,
            ~pointPercent,
            ~refX,
            ~refY,
            ~autoRotate=labelCfg.autoRotate,
          );
        [
          ("x", string_of_int(offsetStyle##x)),
          ("y", string_of_int(offsetStyle##y)),
          ("rotate", string_of_float(offsetStyle##rotate)),
          (
            "textAlign",
            Util.Graphic.getTextAlign(
              ~labelPosition=RE.Label.positionToJs(position),
              ~angle=offsetStyle##angle,
            ),
          ),
        ];
      };

    styles |> Js.Dict.fromList;
  };

  let getLabelStyle = (edge: RE.t, label) => {
    let styles = getLabelStyleByPosition(edge, label);

    let labelStyles =
      switch (label.cfg) {
      | Some(cfg) => cfg.styles
      | None => StylesList.make([])
      };

    Js.Dict.set(styles, "text", label.text);

    [RE.Label.default^.styles, styles, labelStyles]
    |> ReasonViz__Utils.mergeDict;
  };

  let drawLabel = (edge, label) => {
    [%bs.debugger];
    let style = getLabelStyle(edge, label);
    let labelShape =
      Canvas.Group.addShape(edge.group, "text", {"attrs": style});
    Canvas.Shape.set(labelShape, "className", edge.model.shape ++ "label");
    Some(labelShape);
  };

  let draw = (edge: RE.t) => {
    Canvas.Group.clear(edge.group);

    interpolateVertexPoints(edge);

    let shape = drawShape(edge);
    edge.shape = shape;
    let _ = Belt.Option.flatMap(edge.model.label, drawLabel(edge));

    ();
  };
};

module Line =
  Make({
    let getControlPoints = _ => [||];
    let getPath = points => {
      let toPath = (i, point) => {
        let {x, y} = point;
        if (i == 0) {
          {j|M $(x),$(y) |j};
        } else {
          {j|L $(x),$(y) |j};
        };
      };
      Array.mapi(toPath, points) |> Array.to_list |> String.concat("");
    };
  });

module PolyLine =
  Make({
    let getControlPoints = (model: RE.Model.t) => model.controlPoints;

    let getPath = points => {
      let toPath = (i, point) => {
        let {x, y} = point;
        if (i == 0) {
          {j|M $x,$y |j};
        } else {
          {j|L $x,$y |j};
        };
      };
      Array.mapi(toPath, points) |> Array.to_list |> String.concat("");
    };
  });

module Spline =
  Make({
    let getControlPoints = (model: RE.Model.t) => model.controlPoints;

    let getPath = points => {
      let toString = ((s, arr)) => {
        s
        ++ (
          Array.map(string_of_int, arr)
          |> Array.to_list
          |> String.concat(" ")
        );
      };
      let data =
        Array.map(p => [|p.x, p.y|], points) |> Belt.Array.concatMany;

      let path = Canvas.PathUtil.catmullRomToBezier(data);
      let {x, y} = points[0];
      {j|M $x,$y |j}
      ++ (path |> Array.to_list |> List.map(toString) |> String.concat(" "));
    };
  });

module Quadratic =
  Make({
    let curvePosition = 0.5;
    let curveOffset = (-20);

    let getControlPoints = (model: RE.Model.t) => {
      switch (model.controlPoints) {
      | [||] => [|
          Util.Math.getControlPoint(
            ~start=model.sourcePoint,
            ~end_=model.targetPoint,
            ~curvePosition,
            ~curveOffset,
          ),
        |]
      | points => points
      };
    };

    let getPath = points => {
      let {x: x0, y: y0} = points[0];
      let {x: x1, y: y1} = points[1];
      let {x: x2, y: y2} = points[2];

      {j|M $x0,$y0 |j} ++ {j|Q $x1,$y1 $x2,$y2 |j};
    };
  });

module Cubic =
  Make({
    let curvePosition = [|0.5, 0.5|];
    let curveOffset = [|(-20), 20|];

    let getControlPoints = (model: RE.Model.t) => {
      switch (model.controlPoints) {
      | [||] => [|
          Util.Math.getControlPoint(
            ~start=model.sourcePoint,
            ~end_=model.targetPoint,
            ~curvePosition=curvePosition[0],
            ~curveOffset=curveOffset[0],
          ),
          Util.Math.getControlPoint(
            ~start=model.sourcePoint,
            ~end_=model.targetPoint,
            ~curvePosition=curvePosition[1],
            ~curveOffset=curveOffset[1],
          ),
        |]
      | points => points
      };
    };

    let getPath = points => {
      let {x: x0, y: y0} = points[0];
      let {x: x1, y: y1} = points[1];
      let {x: x2, y: y2} = points[2];
      let {x: x3, y: y3} = points[3];

      {j|M $x0,$y0 |j} ++ {j|C $x1,$y1 $x2,$y2 $x3,$y3 |j};
    };
  });