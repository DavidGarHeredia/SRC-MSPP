Base.@kwdef mutable struct Parameters
	numThreadsGurOpti = 2;
	numSolInHeuristic = 8; # you may want to change it with the instance size
	maxIterSearching = 100;
	numThreads = 4;
	arcPenalization = 6000;
	probCoinFlip = 0.5;
	useGurobiWhenSearchingFeasibleSol = 1;
	perNets = 0.05;
	perExtraNets = 0.05;
	minIterIP = 20;
	delta = 0.02;
	gamma = 0.1;
end

parameters = Parameters();

function run_code(inputFiles, param, dictInstanceArcs)
	N = length(inputFiles)
	for idx in 1:N
		files = split(inputFiles[idx], " ");
		nArcs = dictInstanceArcs[files[1]];
		name = split(files[1], "/")[4];
		run(`./executable 
				  $files 
				  $name
				  $nArcs 
				  $(param.numThreadsGurOpti) 
				  $(param.numSolInHeuristic) 
				  $(param.maxIterSearching)
				  $(param.numThreads)
				  $(param.arcPenalization)
				  $(param.probCoinFlip) 
				  $(param.useGurobiWhenSearchingFeasibleSol)
				  $(param.perNets) 
				  $(param.perExtraNets) 
				  $(param.minIterIP) 
				  $(param.delta) 
				  $(param.gamma)`)
	end
end

dictInstanceArcs = Dict("./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv" => 3116931,
						"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv" => 4043046,
						"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv" => 3670644,
						"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv" => 7195904,
						"./data/smallVersion/Small2019May16/Small2019May16.csv" => 9717333,
						"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv" => 8597363,
						"./data/realVersion/2019Jan16/2019Jan16.csv" => 11934419,
						"./data/realVersion/2019May16/2019May16.csv" => 14475487,
						"./data/realVersion/2019Sep16/2019Sep16.csv" => 13993972
						);

inputFiles = [
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/difficult/BLOdif90.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/difficult/BLOdif80.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/difficult/BLOdif70.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/difficult/BLOdif60.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/medium/BLOmed90.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/medium/BLOmed80.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/medium/BLOmed70.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/medium/BLOmed60.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/easy/BLO90.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/easy/BLO80.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/easy/BLO70.csv",
"./data/tinyVersion/Tiny2019Jan16/Tiny2019Jan16.csv ./data/tinyVersion/Tiny2019Jan16/rhs4/easy/BLO60.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/difficult/BLOdif90.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/difficult/BLOdif80.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/difficult/BLOdif70.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/difficult/BLOdif60.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/medium/BLOmed90.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/medium/BLOmed80.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/medium/BLOmed70.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/medium/BLOmed60.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/easy/BLO90.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/easy/BLO80.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/easy/BLO70.csv",
"./data/tinyVersion/Tiny2019May16/Tiny2019May16.csv ./data/tinyVersion/Tiny2019May16/rhs4/easy/BLO60.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/difficult/BLOdif90.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/difficult/BLOdif80.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/difficult/BLOdif70.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/difficult/BLOdif60.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/medium/BLOmed90.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/medium/BLOmed80.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/medium/BLOmed70.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/medium/BLOmed60.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/easy/BLO90.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/easy/BLO80.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/easy/BLO70.csv",
"./data/tinyVersion/Tiny2019Sep16/Tiny2019Sep16.csv ./data/tinyVersion/Tiny2019Sep16/rhs4/easy/BLO60.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/difficult/BLOdif90.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/difficult/BLOdif80.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/difficult/BLOdif70.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/difficult/BLOdif60.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/medium/BLOmed90.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/medium/BLOmed80.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/medium/BLOmed70.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/medium/BLOmed60.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/easy/BLO90.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/easy/BLO80.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/easy/BLO70.csv",
"./data/smallVersion/Small2019Jan16/Small2019Jan16.csv ./data/smallVersion/Small2019Jan16/rhs4/easy/BLO60.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/difficult/BLOdif90.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/difficult/BLOdif80.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/difficult/BLOdif70.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/difficult/BLOdif60.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/medium/BLOmed90.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/medium/BLOmed80.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/medium/BLOmed70.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/medium/BLOmed60.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/easy/BLO90.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/easy/BLO80.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/easy/BLO70.csv",
"./data/smallVersion/Small2019May16/Small2019May16.csv ./data/smallVersion/Small2019May16/rhs4/easy/BLO60.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/difficult/BLOdif90.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/difficult/BLOdif80.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/difficult/BLOdif70.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/difficult/BLOdif60.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/medium/BLOmed90.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/medium/BLOmed80.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/medium/BLOmed70.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/medium/BLOmed60.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/easy/BLO90.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/easy/BLO80.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/easy/BLO70.csv",
"./data/smallVersion/Small2019Sep16/Small2019Sep16.csv ./data/smallVersion/Small2019Sep16/rhs4/easy/BLO60.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/difficult/BLOdif90.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/difficult/BLOdif80.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/difficult/BLOdif70.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/difficult/BLOdif60.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/medium/BLOmed90.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/medium/BLOmed80.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/medium/BLOmed70.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/medium/BLOmed60.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/easy/BLO90.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/easy/BLO80.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/easy/BLO70.csv",
"./data/realVersion/2019Jan16/2019Jan16.csv ./data/realVersion/2019Jan16/rhs4/easy/BLO60.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/difficult/BLOdif90.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/difficult/BLOdif80.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/difficult/BLOdif70.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/difficult/BLOdif60.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/medium/BLOmed90.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/medium/BLOmed80.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/medium/BLOmed70.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/medium/BLOmed60.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/easy/BLO90.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/easy/BLO80.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/easy/BLO70.csv",
"./data/realVersion/2019May16/2019May16.csv ./data/realVersion/2019May16/rhs4/easy/BLO60.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/difficult/BLOdif90.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/difficult/BLOdif80.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/difficult/BLOdif70.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/difficult/BLOdif60.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/medium/BLOmed90.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/medium/BLOmed80.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/medium/BLOmed70.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/medium/BLOmed60.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/easy/BLO90.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/easy/BLO80.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/easy/BLO70.csv",
"./data/realVersion/2019Sep16/2019Sep16.csv ./data/realVersion/2019Sep16/rhs4/easy/BLO60.csv"
]


run_code(inputFiles, parameters, dictInstanceArcs);
