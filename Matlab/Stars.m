%% �������� �������� �������� ����

close all;
clear variables;


%% ���������

lambdaPr = 656.28; %��
speedOfLight = 299792.458; %��/�


%% ������ ������

spectra = importdata ('spectra.csv');
lambdaStart = importdata ('lambda_start.csv');
lambdaDelta = importdata ('lambda_delta.csv');
starNames = importdata ('star_names.csv');


%% ����������� ��������� ���� ����

nObs = size (spectra, 1);
nStars = size (spectra, 2);
lambdaEnd = lambdaStart+(nObs-1)*lambdaDelta;
lambda = (lambdaStart : lambdaDelta : lambdaEnd)';


%% ������ �������� �������� ���� �� �����

for i = 1:nStars
    [sHa(i), idx(i)] = min (spectra(:,i));
    lambdaHa(i) = lambda (idx(i));
    z(i) = (lambdaHa(i)/lambdaPr)-1;
    speed(i,1) = z(i)*speedOfLight;
end
movaway = (starNames (z>0));


%% ���������� �������

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
xlabel('����� �����, ��');
ylabel(['�������������, ���/��^2/�/', char (197)]);
legend(starNames, 'location', 'northeast');
title('������ �����');
text (min (lambda(1))+5, max (spectra(1))+0.15*10^(-13), '����� �������, �01-008');

%% ���������� �������

saveas (fg1, '�������.png');

