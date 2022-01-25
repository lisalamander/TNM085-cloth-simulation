clear
clf
hold on
xlim([0 1])
ylim([0 1])

h = 0.01;
m1 = 1.0;
m2 = 1.0;
k = 30;
c = 0.5;
node(1).p = [0.5 0.6];
node(2).p = [0.3 0.1];

node(1).v = [0 0];
node(2).v = [0 0];

t = 0;
while t < 5
    clf
    hold on
    xlim([0 1])
    ylim([0 1])
    x = node(1).p-node(2).p;
    X = norm(x);
    L = 0.4;

    f_sum1 = -[0 m1*9.82] - c*(node(1).v-node(2).v) + (-1)*k*(X-L)*x/X;
    f_sum2 = -[0 m2*9.82] - c*(node(2).v-node(1).v) + k*(X-L)*x/X;
    node(1).a = 1/m1 * f_sum1;
    node(2).a = 1/m2 * f_sum2;
    node(1) = simpleEuler(node(1),h);
    node(2) = simpleEuler(node(2),h);
    
    for i = 1:2

        if node(i).p(2) < 0
            node(i).v(2) = -1*(node(i).v(2));
            node(i).p(2) = 0;
        end
        if node(i).p(1) < 0
            node(i).v(1) = -1*(node(i).v(1));
            node(i).p(1) = 0;
        end
    end
        plot([node(1).p(1) node(2).p(1)], [node(1).p(2) node(2).p(2)], ".b", 'MarkerSize', 20)
        plot([node(1).p(1) node(2).p(1)], [node(1).p(2) node(2).p(2)], "b");
        pause(h)
        t = t+h;
    
end