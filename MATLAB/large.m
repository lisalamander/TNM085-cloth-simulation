clear
clf
hold on
xlim([0 1])
ylim([0 1])

rows = 3;
cols = 3;

f_g = [0 9.82];
h = 0.01;
% Masses
m = 1;
% Spring constant
k = 30;

% Damping constant
c = 10;
% Initial positions and velocities
for i = 1:rows
    for j = 1:cols
        node(i,j).p = [((j-1)/(cols-1))/2+0.25 ((i-1)/(cols-1))/2+0.5];
        node(i,j).v = [0 0];
        if i == 1
            node(i,j).isFixed = true;
        else
            node(i,j).isFixed = false;
        end
        
    end
end

% Initial time
t = 0;

while t < 5
    clf
    hold on
    xlim([0 1])
    ylim([0 1])
    for i = 1:rows
        for j = 1:cols
            if node(i,j).isFixed == false
                f1 = [0 0];
                f2 = [0 0];
                f3 = [0 0];
                f4 = [0 0];
                f5 = [0 0];
                f6 = [0 0];
                f7 = [0 0];
                f8 = [0 0];
                L = 0.5/(cols-1)-0.1;
                L2 = sqrt(L^2 + L^2);
                % Distances between nodes
                if i > 1 && j > 1
                    x1 = node(i,j).p-node(i-1,j-1).p;
                    X1 = norm(x1);
                    f1 = -k*(X1-L)*x1/X1;
                end
                if j > 1
                    x2 = node(i,j).p-node(i,j-1).p;
                    X2 = norm(x2);
                    f2 = -k*(X2-L)*x2/X2;
                end
                if i < rows && j > 1
                    x3 = node(i,j).p-node(i+1,j-1).p;
                    X3 = norm(x3);
                    f3 = -k*(X3-L)*x3/X3;
                end
                if i < rows
                    x4 = node(i,j).p-node(i+1,j).p;
                    X4 = norm(x4);
                    f4 = -k*(X4-L)*x4/X4;
                end
                if i < rows && j < cols
                    x5 = node(i,j).p-node(i+1,j+1).p;
                    X5 = norm(x5);
                    f5 = -k*(X5-L)*x5/X5;
                end
                if j < cols
                    x6 = node(i,j).p-node(i,j+1).p; %
                    X6 = norm(x6);
                    f6 = -k*(X6-L)*x6/X6;
                end
                if i > 1 && j < cols
                    x7 = node(i,j).p-node(i-1,j+1).p;
                    X7 = norm(x7);
                    f7 = -k*(X7-L)*x7/X7;
                end
                if i > 1
                    x8 = node(i,j).p-node(i-1,j).p;
                    X8 = norm(x8);
                    f8 = -k*(X8-L)*x8/X8;
                end
                f_d  = node(i,j).v*c;
                node(i,j).f_sum = f1+f2+f3+f4+f5+f6+f7+f8-f_g-f_d;
                % Calc acceleration
                node(i,j).a = 1/m * node(i,j).f_sum;
                % Numerical method to get p
                node(i,j) = simpleEuler(node(i,j),h);

                % Check if outside borders
                if node(i,j).p(2) < 0
                    node(i,j).v(2) = -1*(node(i,j).v(2));
                    node(i,j).p(2) = 0;
                elseif node(i,j).p(2) > 1
                    node(i,j).v(2) = -1*(node(i,j).v(2));
                    node(i,j).p(2) = 1;
                end
                if node(i,j).p(1) < 0
                    node(i,j).v(1) = -1*(node(i,j).v(1));
                    node(i,j).p(1) = 0;
                elseif node(i,j).p(1) > 1
                    node(i,j).v(1) = -1*(node(i,j).v(1));
                    node(i,j).p(1) = 1;
                end
            end
        end
    end

    for i = 1:rows
        for j = 1:cols
            plot(node(i,j).p(1), node(i,j).p(2), ".b", 'MarkerSize', 20)
            if i > 1 && j > 1
                plot([node(i,j).p(1) node(i-1,j-1).p(1)], [node(i,j).p(2) node(i-1,j-1).p(2)])
            end
            if j > 1
                plot([node(i,j).p(1) node(i,j-1).p(1)], [node(i,j).p(2) node(i,j-1).p(2)])
            end
            if i < rows
                plot([node(i,j).p(1) node(i+1,j).p(1)], [node(i,j).p(2) node(i+1,j).p(2)])
            end
            %             if i > 1 && j < cols
            %                 plot([node(i,j).p(1) node(i-1,j+1).p(1)], [node(i,j).p(2) node(i-1,j+1).p(2)])
            %             end
        end
    end


    pause(h)
    t = t+h;

end