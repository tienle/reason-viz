module Node = ReasonViz.Node;
module Edge = ReasonViz.Edge;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz.Event;
module ShapeValue = ReasonViz.GraphTypes.ShapeValue;

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
    ~id="node5",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(100.0)),
        ("y", Float(200.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle 5", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node6",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(100.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle 6", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node7",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(300.0)),
        ("size", PairInt(40, 40)),
      ],
    ~anchorPoints=[(0.0, 0.5), (1.0, 0.5)],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle 7", ~position=`bottom, ()),
    (),
  ),
];
let edges = [
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

Event.subscribe(g.events.onBeforePaint, g => Js.log(g));
Event.subscribe(g.events.onBeforeAddNode, n => Js.log(n));
Event.subscribe(g.events.onBeforeAddEdge, e => Js.log(e));

nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);
