function large()
clear
clf
% Settings
rows = 10;
cols = 10;
% Create circle object
circle.r = 0.3;
circle.pos = [0.5 -0.2];
% Create graphical object
S.h = plot(0, 0);
% Create slider
fig = uifigure;
s = uislider(fig);
s.Value = 20;

% Natural length for stretch springs
L = 0.5/(cols-1);
% Natural length for shear springs
L2 = sqrt(L^2 + L^2);
% Natural length for bend springs
L3 = L*2;
% Mass
m = 1;
% Gravity
f_g = m*[0 9.82];
% Step size
h = 0.01;

% Spring constant stretch and shear forces
k = 200;
% Spring constant bend forces
k2 = 50;

% Damping constant
c = 10;
% Initial positions and velocities
for i = 1:rows
    for j = 1:cols
        node(i,j).p = [((j-1)/(cols-1))/2+0.25 ((i-1)/(cols-1))/2+0.5];
        node(i,j).v = [0 0];
        % Fixed nodes 
        if i == 1 
            node(i,j).isFixed = true;
        else
            node(i,j).isFixed = false;
        end
        
    end
end

canvas = createCanvas(node,cols, rows);


while true
    for i = 1:rows
        for j = 1:cols
            if node(i,j).isFixed == false
                % Stretch and shear forces
                f1 = [0 0];
                f2 = [0 0];
                f3 = [0 0];
                f4 = [0 0];
                f5 = [0 0];
                f6 = [0 0];
                f7 = [0 0];
                f8 = [0 0];
                % Ben forces
                fb2 = [0 0];
                fb4 = [0 0];
                fb6 = [0 0];
                fb8 = [0 0];


                % Distances between nodes
                if i > 1 && j > 1
                    x1 = node(i,j).p-node(i-1,j-1).p;
                    X1 = norm(x1);
                    f1 = -k*(X1-L2)*x1/X1;
                end
                if j > 1
                    x2 = node(i,j).p-node(i,j-1).p;
                    X2 = norm(x2);
                    f2 = -k*(X2-L)*x2/X2;
                    if j > 2
                        x2 = node(i,j).p-node(i,j-2).p;
                        X2 = norm(x2);
                        fb2 = -k2*(X2-L3)*x2/X2;    
                    end
                end
                if i < rows && j > 1
                    x3 = node(i,j).p-node(i+1,j-1).p;
                    X3 = norm(x3);
                    f3 = -k*(X3-L2)*x3/X3;
                end
                if i < rows
                    x4 = node(i,j).p-node(i+1,j).p;
                    X4 = norm(x4);
                    f4 = -k*(X4-L)*x4/X4;
                    if i < rows-1
                        x4 = node(i,j).p-node(i+2,j).p;
                        X4 = norm(x4);
                        fb4 = -k2*(X4-L3)*x4/X4;
                    end
                end
                if i < rows && j < cols
                    x5 = node(i,j).p-node(i+1,j+1).p;
                    X5 = norm(x5);
                    f5 = -k*(X5-L2)*x5/X5;
                end
                if j < cols
                    x6 = node(i,j).p-node(i,j+1).p; %
                    X6 = norm(x6);
                    f6 = -k*(X6-L)*x6/X6;
                    if j < cols - 1
                         x6 = node(i,j).p-node(i,j+2).p; %
                        X6 = norm(x6);
                        fb6 = -k2*(X6-L3)*x6/X6;
                    end
                end
                if i > 1 && j < cols
                    x7 = node(i,j).p-node(i-1,j+1).p;
                    X7 = norm(x7);
                    f7 = -k*(X7-L2)*x7/X7;
                end
                if i > 1
                    x8 = node(i,j).p-node(i-1,j).p;
                    X8 = norm(x8);
                    f8 = -k*(X8-L)*x8/X8;
                    if i > 2
                        x8 = node(i,j).p-node(i-2,j).p;
                        X8 = norm(x8);
                        fb8 = -k2*(X8-L3)*x8/X8;
                    end
                end
                % Damping force
                f_d  = node(i,j).v*c;
                % Sum of forces
                node(i,j).f_sum = f1+f2+f3+f4+f5+f6+f7+f8+fb2+fb4+fb6+fb8-f_g-f_d;

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
        % Vertical line, going down %
        % *    *    *               %
        % |                         %
        % *    *    *               %    
        % |                         %
        % *    *    *               %
        for r = rows : -1 : 1
            canvas(index, :) = node(r, c).p;
            index = index + 1;
        end
        % Zig-zag line, going up
         % Vertical line, going down %
        % *    *    *               %
        % |  \                      %
        % * -- *    *               %    
        % |  \                      %
        % * -- *    *               %
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
    
    % Set circle pos
    circle.pos(1) = s.Value / 100-0.5;
    % Draw figure
    drawnow;
  
    
   
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

