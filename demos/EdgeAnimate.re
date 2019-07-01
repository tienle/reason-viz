open ReasonViz__Types;

module Canvas = ReasonViz.Canvas;
module Node = ReasonViz.Node;
module Edge = ReasonViz.Edge;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz.Event;
module GraphTypes = ReasonViz.GraphTypes;
module ShapeValue = GraphTypes.ShapeValue;

let graphOptions =
  ReasonViz.GraphOptions.create(
    ~containerId="index",
    ~width=500,
    ~height=500,
    ~renderer="svg",
    (),
  );

let g: Graph.t = Graph.create(~graphOptions);

let nodes = [
  Node.Model.make(
    ~id="node1",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(100.0)),
        ("y", Float(100.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node2",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(200.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="rect", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(400.0)),
        ("y", Float(100.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle 7", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node4",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(400.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="rect", ~position=`bottom, ()),
    (),
  ),
];
let edges = [
  Edge.Model.make(
    ~source=`NodeId("node1"),
    ~target=`NodeId("node2"),
    ~shape="circle-running",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node3"),
    ~target=`NodeId("node2"),
    ~shape="line-dash",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node2"),
    ~target=`NodeId("node4"),
    ~shape="line-growth",
    (),
  ),
];

module CircleRunningEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;

    let afterDraw = (edge: ReasonViz.Edge.t) => {
      let shape = edge.shape;
      let startPoint = Canvas.Shape.getPoint(shape, ~ratio=0.0);
      let circle =
        Canvas.Group.addShape(
          edge.group,
          "circle",
          {
            "attrs": {
              "fill": "red",
              "r": 3,
              "x": startPoint.x,
              "y": startPoint.y,
            },
          },
        );

      Canvas.Shape.animate(
        circle,
        ~attrs={
          "onFrame": ratio => {
            let point = Canvas.Shape.getPoint(shape, ~ratio);
            point |> pointToJs;
          },
          "repeat": true,
        },
        ~duration=3000,
        (),
      );

      ();
    };
  });

module LineDashEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;
    let lineDash = [|4, 2, 1, 2|];
    let interval = 9;
    let dashArray = [|
      [|0, 1|],
      [|0, 2|],
      [|1, 2|],
      [|0, 1, 1, 2|],
      [|0, 2, 1, 2|],
      [|1, 2, 1, 2|],
      [|2, 2, 1, 2|],
      [|3, 2, 1, 2|],
      [|4, 2, 1, 2|],
    |];

    let afterDraw = (edge: ReasonViz.Edge.t) => {
      let shape = edge.shape;
      let length = Canvas.Shape.getTotalLength(shape);
      let totalDash = ref([||]);

      let size = int_of_float(length) / interval;
      for (i in 0 to size) {
        totalDash := Array.append(totalDash^, lineDash);
      };
      Js.log(totalDash);
      let index = ref(0);

      Canvas.Shape.animate(
        shape,
        ~attrs={
          "onFrame": ratio => {
            index :=
              Int32.rem(Int32.of_int(index^ + 1), Int32.of_int(interval))
              |> Int32.to_int;

            {"lineDash": Belt.Array.concat(dashArray[index^], totalDash^)};
          },
          "repeat": true,
        },
        ~duration=2000,
        (),
      );

      ();
    };
  });

module LineGrowthEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;

    let afterDraw = (edge: ReasonViz.Edge.t) => {
      let shape = edge.shape;
      let length = Canvas.Shape.getTotalLength(shape);

      Canvas.Shape.animate(
        shape,
        ~attrs={
          "onFrame": ratio => {
            let l = ratio *. length;
            {"lineDash": [|l, length -. l|]};
          },
          "repeat": true,
        },
        ~duration=2000,
        (),
      );

      ();
    };
  });

ReasonViz.EdgeShape.register("circle-running", (module CircleRunningEdge));
ReasonViz.EdgeShape.register("line-dash", (module LineDashEdge));
ReasonViz.EdgeShape.register("line-growth", (module LineGrowthEdge));

nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);
