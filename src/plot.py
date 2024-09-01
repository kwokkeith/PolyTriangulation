import matplotlib.pyplot as plt
test_number = int(input())
test_number = str(test_number)
f = open("../tests/in" + test_number + ".txt", "r")
n = int(f.readline())
points = []
xs = []
ys = []
plot1 = plt.figure(1)
for i in range(n):
    x, y = map(float, f.readline().split())
    points.append([x, y])
    xs.append(x)
    ys.append(y)
    # plt.annotate(str(i), [x, y])
# plt.scatter(xs, ys)
for i in range(n):
    xvals = [points[i-1][0], points[i][0]]
    yvals = [points[i-1][1], points[i][1]]
    plt.plot(xvals, yvals)
f.close()

# Flip the polygon about the x-axis
flipped_points = [[x, -y] for x, y in points]

# Calculate the minimum y-value after flipping
min_flipped_y = min(y for _, y in flipped_points)

# Shift the polygon up so that all y-coordinates are non-negative
shifted_points = [[x, y - min_flipped_y] for x, y in flipped_points]

plot2 = plt.figure(2)
f = open("../tests/mout" + test_number + ".txt", "r")
m = int(f.readline())
for i in range(m):
    a, b = map(int, f.readline().split())
    xvals = [points[a][0], points[b][0]]
    yvals = [points[a][1], points[b][1]]
    plt.plot(xvals, yvals)

f.close()

plot3 = plt.figure(3)
f = open("../tests/out" + test_number + ".txt", "r")
m = int(f.readline())
for i in range(m):
    a, b = map(int, f.readline().split())
    xvals = [shifted_points[a][0], shifted_points[b][0]]
    yvals = [shifted_points[a][1], shifted_points[b][1]]
    plt.plot(xvals, yvals)

f.close()

plot4 = plt.figure(4)
f = open("../tests/out" + test_number + ".txt", "r")
m = int(f.readline())
for i in range(m):
    a, b = map(int, f.readline().split())
    xvals = [points[a][0], points[b][0]]
    yvals = [points[a][1], points[b][1]]
    plt.plot(xvals, yvals)

f.close()

plt.show()
