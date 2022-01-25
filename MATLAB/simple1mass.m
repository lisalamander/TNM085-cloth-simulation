clf
hold on
xlim([0 1])
ylim([0 1])

h = 0.01;
m = 1.0;

k = 30;
c = 0.5;
p1 = [0.5 0.6];
p2 = [0 0];

v1 = [0 0];
v2 = [0 0];

t = 0;
while t < 5
    clf
hold on
xlim([0 1])
ylim([0 1])
x = p1-p2;
X = norm(x);
L = 0.4;

f_sum1 = -[0 m*9.82] - c*(v1-v2) + (-1)*k*(X-L)*x/X;
a1 = 1/m * f_sum1;

v1 = v1 + h*a1;
p1 = p1 + h*v1;
if p1(2) < 0
    v1(2) = -v1(2);
    p1(2) = 0;
end
if p1(1) < 0
    v1(1) = -v1(1);
    p1(1) = 0;
end
plot([p1(1) p2(1)], [p1(2) p2(2)], ".b", 'MarkerSize', 20)
plot([p1(1) p2(1)], [p1(2) p2(2)], "b");
pause(h)
t = t+h;
end