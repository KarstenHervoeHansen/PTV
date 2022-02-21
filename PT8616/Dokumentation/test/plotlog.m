## Copyright (C) 2010 JKH
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## plotlog

## Author: JKH <JKH@DKT-JKH>
## Created: 2010-08-02

## Input: Filename
## Plots log file made from PT8616 GPS module

function [ ret ] = plotlog (logfile)

file = fopen(logfile, "r");


[val, count] = fscanf(file, "%f:%f:%f,%f,%f,%f,%f,%f,%f,%f,%f", [11, Inf]);
%[val, count] = fscanf(file, "%f:%f:%f,%f,%f,%f,%f,%f,%f,%f,%f", [11, 3600*10]);

count/11

hrs=1;
mins=2;
secs=3;
phs=4;
cnt=5;
sig1=6;
sig2=7;
sig3=8;
stat=9;
pulse=10;
lock=11;

% phase diff is in steps of 1/148.5 MHz = 6.7 ns
phase = val(phs,:)*1000000000/148500000;

%plot phase diff + signal strength
fig = figure;
subplot(2,1,1); hold on;
plot(phase);
axis([1 count/11 -750 750]);
grid("on");
grid("minor", "on");
title("Phase difference");

subplot(2,1,2); hold on;
axis([1 count/11 0 60]);
plot(val(sig1,:));
plot(val(sig2,:),'r');
plot(val(sig3,:),'g');
grid("on");
title("Signal strengths");

%save plot
outtitle = [logfile(1:sizeof(logfile)-3) 'png'];
print('fig', outtitle,'-dpng');

fclose(file);

endfunction
