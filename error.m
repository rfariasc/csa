%% error: write a file with error
function [outputs] = error(arg1, arg2, arg3)
	figure(20)
	n = arg3;
	filtrada = arg2;
	original = arg1;
	err = 0;
	for	i=1:rows(original)
		err = (original(i)-filtrada(i)/n)^2 + err;
	endfor
	save('-ascii', 'temp.tmp', 'err');
end
	
