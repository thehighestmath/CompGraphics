import numpy as np


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_to(self, point):
        dx = self.x - point.x
        dy = self.y - point.y
        return int((dx ** 2 + dy ** 2) ** 0.5)

    def angle(self, point):
        dx = self.x - point.x
        dy = self.y - point.y
        return np.arctan(dy / dx) / np.pi * 180


if __name__ == '__main__':
    # p1 = Point(81, 59)
    # p2 = Point(81, 176)
    # p3 = Point(194, 59)
    # print(p1.angle(p2))
    # print(p1.angle(p3))

    p1 = Point(151, 8)
    p2 = Point(117, 71)
    p3 = Point(239, 60)
    print(p1.angle(p2))
    print(p1.angle(p3))

    p1 = Point(0, 51)
    p2 = Point(26, 113)
    p3 = Point(119, 0)
    print(p1.angle(p2))
    print(p1.angle(p3))
