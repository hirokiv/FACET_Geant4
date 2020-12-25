clear all
close all

FIGURESETUP;
rm = 0.511; % MeV

% Shows RMS radius, Normalized emittance for each energy range
filepath = '../TestBenchSample/';

specy_list = {
  %  'positron';
    'electron';
    'prim_ele';
    };

for specy_idx = 1:length(specy_list)
    specy = specy_list{specy_idx};
    
    for EMIT= [40] % noramlized emittance
        for RL = [0125] % radiation length
            
            
            pos_file = strcat(filepath, 'beam_',specy,'.txt');
            
            Temp = importdata(pos_file);
            A = sortrows(Temp, 6);
            posx = A(:,1)*1e6; %  radius in x convert to um
            posy =  A(:,2)*1e6;  % radius in y convert to um
            velx =  A(:,3); % in cos angle
            vely =  A(:,4); % in cos angle
            xagl = asin(velx); % radial angle in radian
            ctz = A(:,5)*1e6; % in um
            Ekin = (A(:,6) - 1)*rm ; % Kinetic Energy in MeV
            
            
            %% Calculate energy spectrum
            [N,edges] = histcounts(Ekin);
            Eidx = (edges(2:end) + edges(1:end-1))/2;
            figure(h4); % Show CDF
            plot(Eidx, cumsum(N));
            xlabel('Energy (MeV)');
            ylabel('CDF (Arb.)');
            hold on
                        legend('Electrons', 'Primary electrons only')

            saveas(h4,strcat('g4_',specy,'_CDF.png'))
            
            figure(h3);
            histogram(Ekin);
            xlabel('Energy (MeV)');
            ylabel('Count (Arb.)');
            hold on
            
            legend('Electrons', 'Primary electrons only')
            saveas(h3,strcat('g4_',specy,'_histogram.png'))
            
            %% Calculate emittance
            NUM = 100;
            dibin = round(length(Ekin)/NUM)-1;
            erad = zeros(NUM,1);
            gemit_x = zeros(NUM,1);
            nemit_x = zeros(NUM,1);
            Eidx  = zeros(NUM,1);
            sigx = zeros(NUM,1);
            
            for i = [1:NUM]
                ibegin =  (i-1) *(dibin);
                iend =       i  *(dibin);
                idx = (ibegin+1):iend;
                gemit_x(i) = sqrt(...
                    mean(posx(idx).^2).*mean(xagl(idx).^2) ...
                    - mean(posx(idx).*xagl(idx)).^2 ...
                    );
                sigx(i) = sqrt(mean(posx(idx).^2));
                Eidx(i) = mean(Ekin(idx));
                nemit_x(i) = gemit_x(i) * (Eidx(i)/rm+1);
            end
            gamma_ave = mean(Eidx/rm)
            %% Emittance
            figure(h1);
            semilogx(Eidx,nemit_x);
            xlabel('Energy (MeV)');
            ylabel('Normalized RMS emittance in x (mm-mrad)');
            hold on
            legend('Electrons', 'Primary electrons only')
            saveas(h1,strcat('g4_',specy,'_emittance_x.png'))
            
            %% Radius
            
            figure(h2);
            semilogx(Eidx,sigx);
            xlabel('Energy (MeV)');
            ylabel('Normalized RMS radius in x (um)');
            hold on
                        legend('Electrons', 'Primary electrons only')
            saveas(h2,strcat('g4_',specy,'_radius_x.png'))
            
            
        end
    end
end

