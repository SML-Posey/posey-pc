from pyposey.platform import hardware, io, sensors

imu = sensors.IMUMessage()

ml = io.MessageListener()
ml.add_listener(imu)

imu.message.An = 1
imu.message.Gn = 2
imu.message.Mn = 3
imu.message.Qn = 4
imu.message.Ax = 11
imu.message.Ay = 12
imu.message.Az = 13
imu.message.Gx = 21
imu.message.Gy = 22
imu.message.Gz = 23
imu.message.Mx = 31
imu.message.My = 32
imu.message.Mz = 33
imu.message.Qi = 41
imu.message.Qj = 42
imu.message.Qk = 43
imu.message.Qr = 44
imu.message.Qacc = 45

imu.serialize()
print(imu.buffer.buffer.hex())
print(imu.buffer.valid_checksum)
ml.write(bytes([1,2,3,4,5]))
print('Wrote {}'.format(ml.write(imu)))
print('Used {} of {}'.format(ml.used, ml.capacity))
print('Ignored: {}'.format(ml.num_ignored_messages))
print('Messages: {}'.format(ml.num_messages))
print('Skipped bytes: {}'.format(ml.num_skipped_bytes))

print(ml.process_next())
print('Used {} of {}'.format(ml.used, ml.capacity))
print('Ignored: {}'.format(ml.num_ignored_messages))
print('Messages: {}'.format(ml.num_messages))
print('Skipped bytes: {}'.format(ml.num_skipped_bytes))

imu.message.time = 0
imu.message.An = 0
imu.message.Gn = 0
imu.message.Mn = 0
imu.message.Qn = 0
imu.message.Ax = 0
imu.message.Ay = 0
imu.message.Az = 0
imu.message.Gx = 0
imu.message.Gy = 0
imu.message.Gz = 0
imu.message.Mx = 0
imu.message.My = 0
imu.message.Mz = 0
imu.message.Qi = 0
imu.message.Qj = 0
imu.message.Qk = 0
imu.message.Qr = 0
imu.message.Qacc = 0

print(f'Pre-deserialize : time: {imu.message.time} - An: {imu.message.An} - Gn: {imu.message.Gn} - Mn: {imu.message.Mn} - Qn: {imu.message.Qn} - Ax: {imu.message.Ax} - Ay: {imu.message.Ay} - Az: {imu.message.Az} - Gx: {imu.message.Gx} - Gy: {imu.message.Gy} - Gz: {imu.message.Gz} - Mx: {imu.message.Mx} - My: {imu.message.My} - Mz: {imu.message.Mz} - Qi: {imu.message.Qi} - Qj: {imu.message.Qj} - Qk: {imu.message.Qk} - Qr: {imu.message.Qr} - Qacc: {imu.message.Qacc}')
imu.deserialize()

print(f'Post-deserialize: time: {imu.message.time} - An: {imu.message.An} - Gn: {imu.message.Gn} - Mn: {imu.message.Mn} - Qn: {imu.message.Qn} - Ax: {imu.message.Ax} - Ay: {imu.message.Ay} - Az: {imu.message.Az} - Gx: {imu.message.Gx} - Gy: {imu.message.Gy} - Gz: {imu.message.Gz} - Mx: {imu.message.Mx} - My: {imu.message.My} - Mz: {imu.message.Mz} - Qi: {imu.message.Qi} - Qj: {imu.message.Qj} - Qk: {imu.message.Qk} - Qr: {imu.message.Qr} - Qacc: {imu.message.Qacc}')
