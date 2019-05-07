%% Generate equilibrium distribution to compare with the C++ output
function f_eq_comp(channel_file, f_file, out_file)
    
    % Load the geometry
    Channel2D = load(channel_file);
    
    % Load f
    N_c = 9;
    f = load_f(f_file, N_c);
    
    % Compute the equlibrium distribution function for given distribution
    % function f and geometry
    [Nr Mc N_c]=size(f);
    [iabw1 jabw1]=find(f(:,:,9)~=0);
    lena=length(iabw1);
    ija=(jabw1-1)*Nr+iabw1; % Linear index
    % Density
    rho=sum(f,3);
	% Dot products etc declarations 
    uxsq=zeros(size(rho)); uysq=zeros(size(rho)); usq=zeros(size(rho));
    feq=zeros(size(f));
    ux=zeros(size(rho));uy=zeros(size(rho));
    C_x=[1 0 -1 0 1 -1 -1 1 0];
    C_y=[0 1 0 -1 1 1 -1 -1 0];
    % Density weights
    w0=16/36.;w1=4/36.;w2=1/36.;
    W=[w1 w1 w1 w1 w2 w2 w2 w2 w0];
    cs2=1/3;cs2x2=2*cs2;cs4x2=2*cs2.^2;
    f1=3.; f2=4.5; f3=1.5;
    NxM=Nr*Mc;
        
	% Velocity
    ux=zeros(Nr,Mc);uy=zeros(Nr,Mc);
    for ic=1:N_c-1
        ux=ux+C_x(ic).*f(:,:,ic);uy=uy+C_y(ic).*f(:,:,ic);
    end
   
	% Dot products computation
    ux(ija)=ux(ija)./rho(ija); uy(ija)=uy(ija)./rho(ija);
    uxsq(ija)=ux(ija).^2;uysq(ija)=uy(ija).^2;
    usq(ija)=uxsq(ija)+uysq(ija);

    rt0=w0.*rho; rt1=w1.*rho; rt2=w2.*rho;

    % Equilibrium distribution
    feq(ija)=rt1(ija).*(1+f1*ux(ija)+f2*uxsq(ija)-f3*usq(ija));
    feq(ija+NxM*(2-1))=rt1(ija).*(1+f1*uy(ija)+f2*uysq(ija)-f3*usq(ija));
    feq(ija+NxM*(3-1))=rt1(ija).*(1-f1*ux(ija)+f2*uxsq(ija)-f3*usq(ija));
    feq(ija+NxM*(4-1))=rt1(ija).*(1-f1*uy(ija)+f2*uysq(ija)-f3*usq(ija));

    % Diagonals (X diagonals) (ic-1)
    feq(ija+NxM*(5-1))= rt2(ija) .*(1 +f1*(+ux(ija)+uy(ija)) +f2*(+ux(ija)+uy(ija)).^2 -f3.*usq(ija));
    feq(ija+NxM*(6-1))= rt2(ija) .*(1 +f1*(-ux(ija)+uy(ija)) +f2*(-ux(ija)+uy(ija)).^2 -f3.*usq(ija));
    feq(ija+NxM*(7-1))= rt2(ija) .*(1 +f1*(-ux(ija)-uy(ija)) +f2*(-ux(ija)-uy(ija)).^2 -f3.*usq(ija));
    feq(ija+NxM*(8-1))= rt2(ija) .*(1 +f1*(+ux(ija)-uy(ija)) +f2*(+ux(ija)-uy(ija)).^2 -f3.*usq(ija));

    feq(ija+NxM*(9-1))=rt0(ija).*(1-f3*usq(ija));
    
     % Save the equilibrium distribution function to be used in C++
     save_f(feq, out_file, N_c);
end

function f = load_f(file, Nc)
    %% Loads the distribution function generated with C++ part
    for i = 1:Nc
       f(:,:,i) = load([file, '_', num2str(i-1), '.txt']); 
    end
    % Shift (different conventions)
    f = circshift(f,1,3);
end

function save_f(f, filename, Nc)
    %% Saves the f grid for C++ comparisons
    
    % Shift (different conventions)
    f = circshift(f,1,3);
    
    % Save
    for i = 1:Nc
       temp = f(:,:,i);
       save([filename, '_', num2str(i-1), '.txt'], 'temp', '-ascii'); 
    end
end



