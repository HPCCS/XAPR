import pdb
from sklearn.preprocessing import MinMaxScaler
import statistics as stat
import csv
import numpy as np
from numpy import linalg as LA
from scipy import stats
import matplotlib.pyplot as plt
from sklearn.neural_network import MLPRegressor

###############################################################################
#									      #
#	PREDICTION OF CPU RESILIENCE AND ACQUIRE INHERITED WEIGHTS	      #
#									      #
# #############################################################################
# reading in X, Y

# read in small program labels

y_sr=np.loadtxt(open("small_program_success_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
y_sdc=np.loadtxt(open("small_program_sdc_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
y_ir=np.loadtxt(open("small_program_except_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")

# read in hpc program labels

y_hpc_sr=np.loadtxt(open("hpc_success_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
y_hpc_sdc=np.loadtxt(open("hpc_sdc_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
y_hpc_ir=np.loadtxt(open("hpc_except_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")

# read in small program features

x=np.loadtxt(open("small_program_features.csv", "rb"), delimiter=",", skiprows=0,dtype="float")

# whitening

z_scaler = MinMaxScaler()
x=z_scaler.fit_transform(x)

# read in hpc program features

x_hpc=np.loadtxt(open("hpc_program_features.csv", "rb"), delimiter=",", skiprows=0,dtype="float")

# whitening

x_hpc=z_scaler.fit_transform(x_hpc)

# combine three program lables

y = np.column_stack((y_sr,y_sdc,y_ir))
y_hpc = np.column_stack((y_hpc_sr,y_hpc_sdc,y_hpc_ir))

# run MLP regression model 
# input: x, y, x_hpc
# output: predicted y_hpc with constrains

# hyperparameter settings
hls = (34,10)
#hls = (25,25,25)
act = 'relu'
sl = 'sgd'
lr = 'invscaling' 
lri = 0.001
pt = 0.99
mi = 30000
vb = True
warm_st = False
mt = 0.9
vf = 0.2
bs = 3

mlp = MLPRegressor(hidden_layer_sizes=hls, activation=act, learning_rate_init=lri, solver = sl, learning_rate=lr, power_t=pt, max_iter=mi, verbose=vb, warm_start=warm_st, momentum=mt, validation_fraction=vf,batch_size=bs)
#mlp.__init__(activation=act, solver = sl, learning_rate=lr, power_t=pt, max_iter=mi, verbose=vb, warm_start=warm_st, momentum=mt, validation_fraction=vf)
#mlp.__init__(solver = sl, learning_rate=lr, power_t=pt, max_iter=mi, verbose=vb, warm_start=warm_st, momentum=mt, validation_fraction=vf)
mlp.fit(x,y)
print "number of layers:"
print mlp.n_layers_
print "coefficients: "
mlp_coefs = mlp.coefs_
print mlp.coefs_
print "intercepts: "
mlp_intercepts = mlp.intercepts_
print mlp.intercepts_
print "The number of iterations:"
print mlp.n_iter_
print "Number of outputs:"
print mlp.n_outputs_
print "Name of the output activation function:"
print mlp.out_activation_
pred_y_hpc = mlp.predict(x_hpc)
pred_y_hpc = abs(pred_y_hpc)

print "ground-truth y_hpc: "
print y_hpc
print "predicted y_hpc: "
print pred_y_hpc

# constraint layer function
sum = 0
for i in range(25):
    sum = pred_y_hpc[i][0]+pred_y_hpc[i][1]+pred_y_hpc[i][2]
    pred_y_hpc[i][0] = pred_y_hpc[i][0]/sum
    pred_y_hpc[i][1] = pred_y_hpc[i][1]/sum
    pred_y_hpc[i][2] = pred_y_hpc[i][2]/sum

print "predicted y_hpc with constraints: "
print pred_y_hpc

# prediction error: MAPE
diff = abs(y_hpc - pred_y_hpc)
rel_diff = np.divide(diff,y_hpc)
print "relative diff: "
print rel_diff
mape = np.average(rel_diff)
print "Overall MAPE: "
print mape

# MAPE of success rate
print "SR MAPE: "
mape_sr = np.average(rel_diff[:,0])
print mape_sr

print "SDCR MAPE: "
mape_sdcr = np.average(rel_diff[:,1])
print mape_sdcr

print "IR MAPE: "
mape_ir = np.average(rel_diff[:,2])
print mape_ir


###############################################################################
#									      #
#    TRANSFER LEARNING: PREDICTING GPU RESILIENCE USING INHERITED WEIGHTS     #
#									      #
# #############################################################################
# reading in X, Y

# read in labels for GPU kernels

gy_sr=np.loadtxt(open("gkernel_success_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
gy_sdc=np.loadtxt(open("gkernel_sdc_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")
gy_ir=np.loadtxt(open("gkernel_except_rate_label.csv", "rb"), delimiter=",", skiprows=0, dtype="float")

# read in features for GPU kernels

gx=np.loadtxt(open("gkernel_features.csv", "rb"), delimiter=",", skiprows=0,dtype="float")

# whitening

z_scaler = MinMaxScaler()
gx=z_scaler.fit_transform(gx)

# combine three program lables

gy = np.column_stack((gy_sr,gy_sdc,gy_ir))

# split examples into training set and testing set 

all = np.column_stack((gx,gy))
np.random.shuffle(all)

train = all[:40,:]
test = all[40:,:]

gx_train = train[:,:37]
gx_test = test[:,:37]

gy_train = train[:,37:]
gy_test = test[:,37:]

# read the inherited weights
warm_st = 1 # signaling
new_weights = np.zeros((7,25)) # empty weights for new features
mlp_coefs[0] = np.concatenate((mlp_coefs[0],new_weights)) # inherited weights and empty weights
np.save('./inherit_weights', mlp_coefs)
np.save('./inherit_intercepts', mlp_intercepts)

mlp2 = MLPRegressor(hidden_layer_sizes=hls, activation=act, learning_rate_init=lri, solver = sl, learning_rate=lr, power_t=pt, max_iter=mi, verbose=vb, warm_start=warm_st, momentum=mt, validation_fraction=vf,batch_size=bs,random_state=7)
mlp2.fit(gx_train,gy_train)
pred_gy_test = mlp2.predict(gx_test)
pred_gy_test = abs(pred_gy_test)

print "ground-truth gy_test: "
print gy_test
print "predicted gy_test: "
print pred_gy_test

# constraint layer functions
sum = 0
for i in range(len(gy_test[:,0])):
    sum = pred_gy_test[i][0]+pred_gy_test[i][1]+pred_gy_test[i][2]
    pred_gy_test[i][0] = pred_gy_test[i][0]/sum
    pred_gy_test[i][1] = pred_gy_test[i][1]/sum
    pred_gy_test[i][2] = pred_gy_test[i][2]/sum

print "predicted gy_test with constraints: "
print pred_gy_test

# prediction error: MAPE
gdiff = abs(pred_gy_test-gy_test)
grel_diff = np.divide(gdiff,gy_test)
print "g relative diff: "
print grel_diff
mape = np.average(grel_diff)
print "g Overall MAPE: "
print mape

# MAPE of success rate
print "g SR MAPE: "
mape_sr = np.average(grel_diff[:,0])
print mape_sr

print "g SDCR MAPE: "
mape_sdcr = np.average(grel_diff[:,1])
print mape_sdcr

print "g IR MAPE: "
mape_ir = np.average(grel_diff[:,2])
print mape_ir

