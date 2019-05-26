# use Berman equation
import numpy as np
np.set_printoptions(precision=4)

WL = -10
GW = [
	[WL,WL,WL,WL,WL,WL],
	[WL, 0, 0, 0, 1,WL],
	[WL, 0,WL, 0,-1,WL],
	[WL, 0, 0, 0, 0,WL],
	[WL,WL,WL,WL,WL,WL]
	]

START = (3,1)

P = (0.1,0.8,0.1)
R = np.array(GW)
STATE = np.array(np.where(R != WL)).T
R[R == WL] = 0.0

ACTION = [(0,1),(1,0),(0,-1),(-1,0)]
r_act = [ACTION[-1]] + ACTION + [ACTION[0]]

r = 0.5
V = np.zeros_like(R).astype(np.float)
for i in [0]*1000:
	newV = V.copy()
	for s in STATE:
		pos = tuple(s)
		vs = [V[tuple(s+dest)] for dest in r_act]
		PV = np.max(np.correlate(vs,P))
		newV[pos] = R[pos] + r*PV
	V = newV

print(V)
