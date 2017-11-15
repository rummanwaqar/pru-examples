input_pins=()
output_pins=(P9.27)

for pin in "${input_pins[@]}"
do
	config-pin -a $pin pruin
	config-pin -q $pin
done

for pin in "${output_pins[@]}"
do
	config-pin -a $pin pruout
	config-pin -q $pin
done 
