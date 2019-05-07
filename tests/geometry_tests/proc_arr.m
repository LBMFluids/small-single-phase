function proc_arr(fin, fout)
    % Turn array of objects from file fin into arrays of single points 
    % and save those as text files fout
    
    data = load(fin);
    cnv = double(~bwmorph(~data,'shrink',Inf));
    save(fout, 'cnv','-ascii')
end

