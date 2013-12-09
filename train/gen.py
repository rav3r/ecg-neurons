import random

data_size = 100
print str(data_size) + ' 3 1'

for i in range(100):
    t_height = random.uniform(-1, 1)
    q_time = random.uniform(0.02, 0.06)
    qr_perc = random.uniform(0.15, 0.35)
    print str(t_height) + ' ' + str(q_time) + ' ' + str(qr_perc)
    if t_height < 0 or q_time > 0.04 or qr_perc > 0.25:
        print 1
    else:
        print -1
