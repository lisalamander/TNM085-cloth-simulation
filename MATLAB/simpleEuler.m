function res = simpleEuler(node, h)
    res = node;
    res.v = node.v + h*node.a;
    res.p = node.p + h*res.v;
    

end