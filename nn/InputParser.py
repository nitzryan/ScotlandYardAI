def ParseLocationPredictionFile(filename):
    try:
        this_input = []
        this_output = 0
        
        game_inputs = []
        game_outputs= []
        
        all_inputs = []
        all_outputs = []
        
        inputs_done = False
        with open(filename, "rb") as file:
            while True:
                byte = file.read(1)
                byte = int.from_bytes(byte, byteorder='big')
                
                if byte == 255:
                    all_inputs.append(game_inputs)
                    all_outputs.append(game_outputs)
                    game_inputs = []
                    game_outputs = []
                    break
                elif byte == 254:
                    game_inputs.append(this_input)
                    game_outputs.append(this_output)
                    this_input = []
                elif byte == 253:
                    inputs_done = True
                elif inputs_done:
                    this_output = byte
                    inputs_done = False
                else:
                    this_input.append(byte)
        
        print("Inputs")
        print(all_inputs)
        print("Outputs")
        print(all_outputs)
    except Exception as e:
        print(e)