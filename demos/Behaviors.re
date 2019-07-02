open ReasonViz__Types;

module Canvas = ReasonViz.Canvas;
module Node = ReasonViz.Node;
module Edge = ReasonViz.Edge;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz.Event;
module GraphTypes = ReasonViz.GraphTypes;
module ShapeValue = GraphTypes.ShapeValue;
module Util = ReasonViz.Util;

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
        ("y", Float(200.0)),
        ("size", PairInt(40, 40)),
      ],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node2",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(200.0)),
        ("size", PairInt(40, 40)),
      ],
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(300.0)),
        ("size", PairInt(40, 40)),
      ],
    (),
  ),
];
let edges = [
  Edge.Model.make(
    ~source=`NodeId("node1"),
    ~target=`NodeId("node2"),
    ~shape="cubic",
    ~label=Edge.Label.make(~text="cubic", ~refY=10, ()),
    (),
  ),
];

module ClickToSelect =
  ReasonViz.Behavior.ClickToSelect({
    let selected = Style.(make([fill("indigo"), stroke("#333")]));
    let normal = Style.(make([fill("#fff"), stroke("#333")]));
  });

module DragNode =
  ReasonViz.Behavior.DragNode({
    let draggingStyle =
      Util.makeObj({
        "lineWidth": 3,
        "strokeOpacity": 0.8,
        "fill": "#F3F9FF",
        "fillOpacity": 0.5,
      });
  });

ClickToSelect.activate(g);
DragNode.activate(g);

nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);
