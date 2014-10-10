%% elo330: elo330 octave function
function [outputs] = elo330(arg);
	% printf('entre\n')
	graph = transpose(arg);
	x= 0:(1/8000):((columns(graph)-1)/8000);
	count = 0;
	j = 1;
	pivot= 0;
	negativePivot = 0;


	for i=1:columns(graph)
		if graph(i)>=32750
		pivot(j) = i;
		j= j+1; 
		end
	endfor

	e = 1;
	for i=1:columns(graph)
		if graph(i)<=-32700
		negativePivot(e) = i;
		e= e+1; 
		end
	endfor


	windowsLeft = 0;
	windowsRight= 0;
	polymem = 0;
	for i=1:columns(negativePivot)
		windowsRight = negativePivot(i)+5;
		windowsLeft  = negativePivot(i)-3;
		if(windowsRight>columns(x))
			continue
		end
		if(negativePivot(i)>polymem || i ==1 )

			xAux= x(windowsLeft:windowsRight);
			yAux= graph(windowsLeft:windowsRight);
			% printf('hola\n')
			po = polyfit(xAux,yAux,4);
			gr = polyval(po,xAux);
			graph(windowsLeft:windowsRight) = gr;
			polymem = windowsRight-1;
		end
	endfor


	windowsLeft = 0;
	windowsRight= 0;
	polymem = 0;
	for i=1:columns(pivot)
		windowsRight = pivot(i)+7;
		windowsLeft  = pivot(i)-3;
		if(windowsRight>columns(x))
			continue
		end
		if(pivot(i)>polymem || i ==1 )

			xAux= x(windowsLeft:windowsRight);
			yAux= graph(windowsLeft:windowsRight);
			% printf('hola\n')
			po = polyfit(xAux,yAux,4);
			gr = polyval(po,xAux);
			graph(windowsLeft:windowsRight) = gr;
			polymem = windowsRight-1;
		end
	endfor
	outputs = graph;
end
