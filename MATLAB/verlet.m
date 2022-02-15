function res = verlet(node, h)
     
    res = node;
    
    res.p = 2*node.p - node.prev_p + h*h*node.a;     
    res.prev_p = node.p;                             
    res.v = 1/(2*h)*(res.p - res.prev_p);  
     

end