clear
clf
hold on
xlim([0 1])
ylim([0 1])

h = 0.01;
% Masses
m = [1 1 1 1];
% Spring constant
k = 30;
% Damping constant
c = 5;
% Initial positions
node(1).p = [0.5 0.1];
node(2).p = [0.1 0.2];
node(3).p = [0.2 0.6];
node(4).p = [0.6 0.5];
% Initial velocities
for i = 1:4
    node(i).v = [0 0];
end
% Initial time
t = 0;
while t < 5
    clf
    hold on
    xlim([0 1])
    ylim([0 1])
    % Distances between nodes
    x1 = node(1).p-node(2).p;
    X1 = norm(x1);
    x2 = node(2).p-node(3).p;
    X2 = norm(x2);
    x3 = node(3).p-node(4).p;
    X3 = norm(x3);
    x4 = node(4).p-node(1).p;
    X4 = norm(x4);
    L = 0.4;
    % Diff between velocities
    v1 = node(1).v-node(2).v;
    v2 = node(2).v-node(3).v;
    v3 = node(3).v-node(4).v;
    v4 = node(4).v-node(1).v;
    % Forces between nodes
    f12 = k*(X1-L)*x1/X1;
    f23 = k*(X2-L)*x2/X2;
    f34 = k*(X3-L)*x3/X3;
    f41 = k*(X4-L)*x4/X4;
    f_g = [0 9.82];
    % Damping forces
    fd12 = c*v1;
    fd23 = c*v2;
    fd34 = c*v3;
    fd41 = c*v4;
    % Sum of forces for each node
    node(1).f_sum = -f_g - f12 + f41 - (fd12 - fd41);
    node(2).f_sum = -f_g + f12 - f23 - (-fd12 + fd23);
    node(3).f_sum = -f_g + f23 - f34 - (-fd23 + fd34);
    node(4).f_sum = -f_g + f34 - f41 - (-fd34 + fd41);
    % Numerical method for each nodes acceleration to position
    for i = 1:4
        node(i).a = 1/m(i) * node(i).f_sum;
        node(i) = simpleEuler(node(i),h);
    end

    for i = 1:4
        % Check if inside borders
        if node(i).p(2) < 0
            node(i).v(2) = -1*(node(i).v(2));
            node(i).p(2) = 0;
        elseif node(i).p(2) > 1
            node(i).v(2) = -1*(node(i).v(2));
            node(i).p(2) = 1;
        end
        if node(i).p(1) < 0
            node(i).v(1) = -1*(node(i).v(1));
            node(i).p(1) = 0;
        elseif node(i).p(1) > 1
            node(i).v(1) = -1*(node(i).v(1));
            node(i).p(1) = 1;
        end
    end

    for i = 1:4
        x_plot(i) = node(i).p(1);
        y_plot(i) = node(i).p(2);
    end
    x_plot(4+1) = node(1).p(1);
    y_plot(4+1) = node(1).p(2);
    plot(x_plot, y_plot, ".b", 'MarkerSize', 20)
    plot(x_plot, y_plot, "b");
    pause(h)
    t = t+h;

end