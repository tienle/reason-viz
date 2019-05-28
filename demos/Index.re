open ReasonViz.Types;
module Style = ReasonViz.Style;
module Data = ReasonViz.Data;
module Graph = ReasonViz.Graph;

let data: Data.t('a) = {
  nodes: [
    Data.node(
      ~id="node1",
      ~shape=`Circle({x: 100, y: 0, radius: 20}),
      ~styles=Style.[fill("red"), stroke("yellow")],
      (),
    ),
    Data.node(
      ~id="node2",
      ~shape=`Circle({x: 200, y: 200, radius: 50}),
      ~styles=Style.[fill("red"), stroke("yellow")],
      (),
    ),
  ],
  /* edges: [Data.edge(~source="node1", ~target="node2", ())], */
};

let graphOptions =
  ReasonViz.GraphOptions.create(
    ~containerId="index1",
    ~width=500,
    ~height=500,
    (),
  );

let g = Graph.create(~graphOptions);
Graph.parse(g, ~data);
Graph.render(g);
