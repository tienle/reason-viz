open ReasonViz__Types;

module Node = ReasonViz__Node;
module Edge = ReasonViz__Edge;
module ShapeValue = Node.ShapeValue;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz__Event;

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
        ("x", Float(200.0)),
        ("y", Float(150.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black"), fill("#fe3")],
    ~label=Node.Label.make(~text="rect", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="ellipse",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(100.0)),
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
        ("x", Float(400.0)),
        ("y", Float(100.0)),
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
    ~source=`NodeId("node1"),
    ~target=`NodeId("node2"),
    ~label=Edge.Label.make(~text="line", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 250.0, y: 200.0}),
    ~target=`Point({x: 360.0, y: 200.0}),
    ~controlPoints=[|{x: 300.0, y: 220.0}|],
    ~shape="spline",
    ~label=Edge.Label.make(~text="polyline", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 100.0, y: 300.0}),
    ~target=`Point({x: 200.0, y: 300.0}),
    ~shape="quadratic",
    ~label=Edge.Label.make(~text="quadratic", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 250.0, y: 300.0}),
    ~target=`Point({x: 360.0, y: 300.0}),
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
            ("x", Float(0.0 +. float_of_int(x) *. 20.0)),
            ("y", Float(0.0 +. float_of_int(x) *. 20.0)),
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
ReasonViz__EdgeShape.register(
  "cubic-horizontal",
  (module ReasonViz__EdgeShape.CubicHorizontal),
);

let graphOptions =
  ReasonViz.GraphOptions.create(
    ~containerId="index1",
    ~width=1000,
    ~height=1000,
    ~renderer="svg",
    (),
  );

let g = Graph.create(~graphOptions);
nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);

let graphOptions2 =
  ReasonViz.GraphOptions.create(
    ~containerId="index2",
    ~width=1000,
    ~height=1000,
    ~renderer="svg",
    (),
  );

let g2: Graph.t = Graph.create(~graphOptions=graphOptions2);

let nodes2 = [
  Node.Model.make(
    ~id="node5",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(300.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node6",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(400.0)),
        ("y", Float(250.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node7",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(400.0)),
        ("y", Float(350.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
];
let edges2 = [
  Edge.Model.make(
    ~source=`NodeId("node5"),
    ~target=`NodeId("node6"),
    ~label=Edge.Label.make(~text="cubic", ()),
    ~shape="cubic-horizontal",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node5"),
    ~target=`NodeId("node7"),
    ~label=Edge.Label.make(~text="cubic", ()),
    ~shape="cubic",
    (),
  ),
];
Event.subscribe(g2.events.onBeforePaint, g => Js.log(g));
Event.subscribe(g2.events.onBeforeAddNode, n => Js.log(n));
Event.subscribe(g2.events.onBeforeAddEdge, e => Js.log(e));
nodes2 |> Graph.addNodes(g2);
edges2 |> Graph.addEdges(g2);
Graph.render(g2);
