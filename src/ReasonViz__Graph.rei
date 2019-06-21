type t;

let create: (~graphOptions: ReasonViz__Graph_Options.t) => t;
let addNodes: (t, list(ReasonViz__Node.Model.t)) => unit;
let addEdges: (t, list(ReasonViz__Edge.Model.t)) => unit;
let render: t => unit;
