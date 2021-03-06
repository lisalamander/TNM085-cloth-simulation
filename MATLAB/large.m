function large()
clear
clf

rows = 10;
cols = 10;

circle.r = 0.2;
circle.pos = [0.5 -0.5];
S.h = plot(0, 0);
test = [0 0];
L = 0.5/(cols-1);
L2 = sqrt(L^2 + L^2);
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

canvas = createCanvas(node,cols, rows);
% Initial time
t = 0;

while t < 5
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
                if node(i,j).p(2) < -1
                    node(i,j).v(2) = -1*(node(i,j).v(2));
                    node(i,j).p(2) = -1;
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

                % Sphere intersection
                nodeToCircle = node(i,j).p-circle.pos;
                distToCircle = norm(nodeToCircle);
                if  distToCircle < circle.r
                    circleBorder = circle.r * nodeToCircle / distToCircle;
                    node(i,j).p = circle.pos + circleBorder;
                    node(i,j).v = -1*node(i,j).v;
                end
            end
        end
    end
    index = 1;
    for c = 1 : cols
        % Vertical line, going down
        for r = rows : -1 : 1
            canvas(index, :) = node(r, c).p;
            index = index + 1;
        end

        % Zig-zag line, going up
        for r = 1 : rows
            canvas(index,:) = node(r,c).p;
            index = index + 1;
            if (c < cols)
                canvas(index ,:) = node(r, c + 1).p;
                index = index + 1;
            end
        end

    end
    
    set(S.h, 'XData', canvas(:,1));
    set(S.h, 'YData', canvas(:,2));
    
    drawnow;
  
    pause(h)
    t = t+h;
end



end
%%
function canvas = createCanvas(nodes,cols,rows)
% Graphic thingy
index = 1;
for c = 1 :cols
    for r = 1 : rows
        canvas(index,:) = nodes(r, c).p;
        index = index + 1;
    end
end

canvas_min = min(canvas);
canvas_max = max(canvas);
range = canvas_max - canvas_min;

xlim([canvas_min(1)-range(1) canvas_max(1)+range(1)])
ylim([canvas_min(2)-range(2)*3 canvas_max(2)+range(2)])
end

