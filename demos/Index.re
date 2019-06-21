open ReasonViz__Types;

module Node = ReasonViz__Node;
module Edge = ReasonViz__Edge;
module ShapeValue = Node.ShapeValue;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;

let nodes = [
  Node.Model.make(
    ~id="node1",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Int(100)),
        ("y", Int(100)),
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
        ("x", Int(200)),
        ("y", Int(100)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="rect", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="ellipse",
    ~props=
      ShapeValue.[
        ("x", Int(300)),
        ("y", Int(100)),
        ("size", PairInt(60, 30)),
      ],
    ~styles=[],
    ~label=Node.Label.make(~text="ellipse", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node4",
    ~shape="image",
    ~props=
      ShapeValue.[
        ("x", Int(400)),
        ("y", Int(100)),
        ("size", PairInt(40, 40)),
        (
          "img",
          Str(
            "https://gw.alipayobjects.com/zos/rmsportal/XuVpGqBFxXplzvLjJBZB.svg",
          ),
        ),
      ],
    ~styles=[],
    ~label=Node.Label.make(~text="image", ~position=`bottom, ()),
    (),
  ),
];

let edges = [
  Edge.Model.make(
    ~source=`Point({x: 100, y: 200}),
    ~target=`Point({x: 200, y: 200}),
    ~label=Edge.Label.make(~text="line", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 250, y: 200}),
    ~target=`Point({x: 360, y: 200}),
    ~controlPoints=[|{x: 300, y: 220}|],
    ~shape="spline",
    ~label=Edge.Label.make(~text="polyline", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 100, y: 300}),
    ~target=`Point({x: 200, y: 300}),
    ~shape="quadratic",
    ~label=Edge.Label.make(~text="quadratic", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 250, y: 300}),
    ~target=`Point({x: 360, y: 300}),
    ~shape="cubic",
    ~label=Edge.Label.make(~text="cubic", ~refY=10, ()),
    (),
  ),
];

let mynodes = ref([]);

for (x in 4 to 400) {
  mynodes :=
    [
      Node.Model.make(
        ~id="node" ++ string_of_int(x),
        ~props=
          ShapeValue.[
            ("x", Int(0 + x * 20)),
            ("y", Int(0 + x * 20)),
            ("size", PairInt(20, 20)),
          ],
        ~styles=Style.[fill("green"), stroke("red")],
        ~shape="circle",
        ~label=Node.Label.make(~text="node", ~position=`center, ()),
        (),
      ),
      ...mynodes^,
    ];
};

ReasonViz__NodeShape.register("circle", (module ReasonViz__NodeShape.Circle));
ReasonViz__NodeShape.register("rect", (module ReasonViz__NodeShape.Rect));
ReasonViz__NodeShape.register(
  "ellipse",
  (module ReasonViz__NodeShape.Ellipse),
);
ReasonViz__NodeShape.register("image", (module ReasonViz__NodeShape.Image));
ReasonViz__EdgeShape.register("line", (module ReasonViz__EdgeShape.Line));
ReasonViz__EdgeShape.register(
  "polyline",
  (module ReasonViz__EdgeShape.PolyLine),
);
ReasonViz__EdgeShape.register("spline", (module ReasonViz__EdgeShape.Spline));
ReasonViz__EdgeShape.register(
  "quadratic",
  (module ReasonViz__EdgeShape.Quadratic),
);
ReasonViz__EdgeShape.register("cubic", (module ReasonViz__EdgeShape.Cubic));

let graphOptions =
  ReasonViz.GraphOptions.create(
    ~containerId="index1",
    ~width=1000,
    ~height=1000,
    ~renderer="canvas",
    (),
  );

let g = Graph.create(~graphOptions);
nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);

module Util = ReasonViz__Utils;
let _ = {
  let m =
    Util.Math.getCircleIntersectByPoint(
      {"x": 100, "y": 100, "r": 20.5},
      {x: 250, y: 220},
    )
    |> Belt.Option.getExn;
  Js.log @@ m;
  Js.log @@ m.x;
  Js.log @@ m.y;
};
