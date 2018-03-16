%% This is a simple example to load and plot and "easy" file.
% data from StarStim device, using tDCS. Channels 1 and 2 are marked as
% "-1" as they are used for stimulation. 

filename='20131014162631_ExpTest-BF1.easy';
Channel=4;

d=load(filename);
%The data is now loaded as an array in the "d" variable.

% Next we define the time axis using the last column in the data (in ms
% Unix time):
time=d(:,end); % time stamp is in the last column - in ms Unix time
time=time-time(1); % set clock to zero in first sample
time=time/1000; % change time units to seconds
 
 
%'''Example one:''' we plot channel 1 in mV 
 
figure(1); 
plot(time, d(:,Channel)/1e6); % divide by one million to go to mV
xlabel('Time from start (s)');
title('Channel data');
ylabel('Voltage (mV)');

% '''Example two:''' we plot channels 1 to 8 in uV
figure(2); plot(time, d(:,1:8)/1e3); % divide by one thousand to go to uV
xlabel('Time from start (s)')
ylabel('Voltage (uV)');
legend({'Ch1','Ch2','Ch3','Ch4','Ch5','Ch6','Ch7','Ch8'});
title('Channels 1-8 data');