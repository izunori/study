'''
Simple Monte Carlo method and Importance sampling
'''

import numpy as np
from scipy.stats import norm

thres = 3.5
N = 10000

# Simple Monte Carlo method
vs = norm.rvs(size=N)
samples = vs[vs > thres]
p_MC = samples.size/N
print("MC:",p_MC)

# Importance sampling method
loc = thres
vs = norm.rvs(loc=loc, size=N)
samples = vs[vs > thres]
L = norm.pdf(samples)/norm.pdf(samples,loc=loc) # likelihood ratio
p_IS = np.sum(L)/N
print("IS:",p_IS)

# Scipy
p_Scipy = 1-norm.cdf(thres)
print("Scipy:",p_Scipy)


