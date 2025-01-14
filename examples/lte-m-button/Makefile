soralet_id ?= lte-m-button
soracom_profile ?= default
output = lte-m-button.wasm


objs = ../../soracom/orbit.o main.o


.PHONY: build
build: $(output)

$(output): $(objs)
	emcc $(objs) --no-entry -s ERROR_ON_UNDEFINED_SYMBOLS=0 -Os -o $(output)

../../soracom/orbit.o:
	$(MAKE) -C ../../soracom/

main.o: main.cpp
	emcc -I../../ -std=c++23 -c main.cpp -o main.o

.PHONY: clean
clean:
	rm -f $(output) $(objs)

.PHONY: create-soralet
create-soralet:
	soracom --profile $(soracom_profile) soralets create --soralet-id $(soralet_id)

.PHONY: upload
upload:
	soracom --profile $(soracom_profile) soralets upload --soralet-id $(soralet_id) --content-type application/octet-stream --body @$(output)

body = '{"source":{"resourceType":"Subscriber","resourceId":"001019999999999"},"tags":{"name":"test name"},"location":{"lat":43.12345,"lon":138.112233}}'
payload = '{"batteryLevel":1,"clickType":1,"clickTypeName":"SINGLE"}'

.PHONY: test
test:
	soracom --profile $(soracom_profile) soralets exec --soralet-id $(soralet_id) --version "\$$LATEST" --direction uplink --content-type "application/json" --payload $(payload) --body $(body)

.PHONY: log
log:
	soracom --profile $(soracom_profile) soralets get-logs --soralet-id $(soralet_id)
