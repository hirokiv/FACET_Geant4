clear all
close all

FIGURESETUP;
rm = 0.511; % MeV

% Shows RMS radius, Normalized emittance for each energy range
filepath = '../TestBenchSample/';
specy = 'electron';

for EMIT= [40]
    for RL = [0125]
        
      
        pos_file = strcat(filepath, 'beam_',specy,'.txt');
        
        Temp = importdata(pos_file);
        A = sortrows(Temp, 6);
        posr = sqrt(A(:,1).^2 + A(:,2).^2)*1e6; % Convert to um
        velr = sqrt(A(:,3).^2 + A(:,4).^2); % in cos angle
        ragl = asin(velr); % radial angle in radian
        %ctz = A(:,4); % in cm
        Ekin = (A(:,6) - 1)*rm ; % Kinetic Energy in MeV
        
        
        %% Calculate energy spectrum
        [N,edges] = histcounts(Ekin);
        Eidx = (edges(2:end) + edges(1:end-1))/2;
        figure(h4); % Show CDF
        plot(Eidx, cumsum(N));
        xlabel('Energy (MeV)');
        ylabel('CDF (Arb.)');
        hold on
        saveas(h4,strcat('g4_',specy,'_CDF.png'))
        
        figure(h3);
        histogram(Ekin);
        xlabel('Energy (MeV)');
        ylabel('Count (Arb.)');
        hold on
        saveas(h3,strcat('g4_',specy,'_histogram.png'))
        
        %% Calculate emittance
        NUM = 50;
        dibin = round(length(Ekin)/NUM)-1;
        erad = zeros(NUM,1);
        gemit = zeros(NUM,1);
        nemit = zeros(NUM,1);
        Eidx  = zeros(NUM,1);
        sigr = zeros(NUM,1);
        
        for i = [1:NUM]
            ibegin =  (i-1) *(dibin);
            iend =       i  *(dibin);
            idx = (ibegin+1):iend;
            gemit(i) = sqrt(...
                mean(posr(idx).^2).*mean(ragl(idx).^2) ...
                - mean(posr(idx).*ragl(idx)).^2 ...
                );
            sigr(i) = sqrt(mean(posr(idx).^2));
            Eidx(i) = mean(Ekin(idx));
            nemit(i) = gemit(i) * (Eidx(i)/rm+1);
        end
        gamma_ave = mean(Eidx/rm)
        %% Emittance
        figure(h1);        
        loglog(Eidx,nemit);
        xlabel('Energy (MeV)');
        ylabel('Normalized RMS emittance (mm-mrad)');
        hold on
        saveas(h1,strcat('g4_',specy,'_emittance.png'))
        
        %% Radius
        
        figure(h2);
        loglog(Eidx,sigr);
        xlabel('Energy (MeV)');
        ylabel('Normalized RMS radius (um)');
        hold on
        saveas(h2,strcat('g4_',specy,'_radius.png'))
        
        
    end
end
