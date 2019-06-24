module Node = ReasonViz.Node;
module Edge = ReasonViz.Edge;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz.Event;
module ShapeValue = Node.ShapeValue;

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
        ("y", Float(100.0)),
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
    ~styles=Style.[stroke("red")],
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
    ~source=`Point({x: 100.0, y: 200.0}),
    ~target=`Point({x: 200.0, y: 200.0}),
    ~label=Edge.Label.make(~text="line", ~refY=10, ()),
    (),
  ),
  Edge.Model.make(
    ~source=`Point({x: 250.0, y: 200.0}),
    ~target=`Point({x: 360.0, y: 200.0}),
    ~controlPoints=[|{x: 300.0, y: 220.0}|],
    ~styles=Style.[stroke("blue")],
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
