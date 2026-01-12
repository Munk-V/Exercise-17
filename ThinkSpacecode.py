channelID = 3226621;
readKey   = 'OJ5ZVN52ZDI0KWLR';    % private? sæt din read key her
N = 200;

if strlength(readKey) == 0
    [data, time] = thingSpeakRead(channelID, 'Fields', [1 2], 'NumPoints', N);
else
    [data, time] = thingSpeakRead(channelID, 'Fields', [1 2], 'NumPoints', N, 'ReadKey', readKey);
end

rssi = data(:,1);
led  = data(:,2);

% fjern NaN
valid = ~isnan(rssi) & ~isnan(led);
time = time(valid);
rssi = rssi(valid);
led  = led(valid);

% RSSI sanity (valgfrit)
rssi(rssi > 0) = -rssi(rssi > 0);

clf
yyaxis left
plot(time, rssi, '-', 'LineWidth', 1.5)
ylabel('RSSI (dBm)')
ylim([-100 -30])

yyaxis right
plot(time, led, 'o', 'MarkerSize', 6, 'LineWidth', 1.5)  % tydelige punkter
ylabel('LED (0/1)')
ylim([-0.2 1.2])

title('Exercise 17c: RSSI + LED (ThingSpeak read)')
xlabel('Time')
grid on