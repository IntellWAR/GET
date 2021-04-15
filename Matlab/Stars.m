%% Изучение красного смещения звёзд

close all;
clear variables;


%% Константы

lambdaPr = 656.28; %нм
speedOfLight = 299792.458; %км/с


%% Импорт данных

spectra = importdata ('spectra.csv');
lambdaStart = importdata ('lambda_start.csv');
lambdaDelta = importdata ('lambda_delta.csv');
starNames = importdata ('star_names.csv');


%% Определение диапазона длин волн

nObs = size (spectra, 1);
nStars = size (spectra, 2);
lambdaEnd = lambdaStart+(nObs-1)*lambdaDelta;
lambda = (lambdaStart : lambdaDelta : lambdaEnd)';


%% Расчёт скорости удаления звёзд от Земли

for i = 1:nStars
    [sHa(i), idx(i)] = min (spectra(:,i));
    lambdaHa(i) = lambda (idx(i));
    z(i) = (lambdaHa(i)/lambdaPr)-1;
    speed(i,1) = z(i)*speedOfLight;
end
movaway = (starNames (z>0));


%% Построение графика

fg1 = figure;
hold on;

for i = 1:nStars
    if z(i) > 0 
        plot(lambda, spectra (:,i), 'LineWidth',3);
    else 
        plot(lambda, spectra (:,i), '--', 'LineWidth',1);
    end
end

grid on;
xlabel('Длина волны, нм');
ylabel(['Интенсивность, эрг/см^2/с/', char (197)]);
legend(starNames, 'location', 'northeast');
title('Спектр зведы');
text (min (lambda(1))+5, max (spectra(1))+0.15*10^(-13), 'Павел Корнеев, Б01-008');

%% Сохранение графика

saveas (fg1, 'спектры.png');

