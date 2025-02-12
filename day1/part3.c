#include <stdio.h>
#include<stdlib.h>
#include <iio.h>
#include <unistd.h>

#define URI "ip:10.76.84.205"

int main() {

	unsigned int major;
	unsigned int minor;
	char git_tag[8];
	const char *description;
	const char *ctx_name;
	const char *ctx_val;
	const char *dev_name;
	const char *dev_val;
	struct iio_context *ctx;
	struct iio_device *dev;
	struct iio_channel *chn;
	iio_library_get_version(&major, &minor, git_tag);

	printf("libiio version: %d.%d - %s \n", major,minor,git_tag);

	ctx = iio_create_context_from_uri(URI);

	description = iio_context_get_description(ctx);
	printf("Description: %s\n" , description);
    
	ctx = iio_context_get_devices_count(URI);

	if (ctx == NULL){
		printf("The board is not available!\n");
		return 1;
	}
    
	// Obtin numarul de contexte
	int attr_count = iio_context_get_attrs_count(ctx);
	
	for (int i = 0; i < attr_count; i++)
	{
	iio_context_get_attr(ctx,i,&ctx_name,&ctx_val); 
	printf("ctx attr %d: %s - %s\n" ,i, ctx_name, ctx_val);
	}
	printf("\n");
	
	dev = iio_context_find_device(ctx, "ad5592r_s");
	chn = iio_device_get_channel(dev, 0);

	int chn_attr_count = iio_device_get_attrs_count(dev);

	printf("Channel0 attributes: ");
	for (int i = 0; i < chn_attr_count; i++)
	{
		printf(iio_channel_get_attr(chn, i));
	}
	
	const char *attr_name = iio_channel_get_attr(chn, 0);
	char raw_data;
	char *raw_data_pointer = (char*)malloc(50 * sizeof(char));
	iio_channel_attr_read(chn, attr_name, raw_data_pointer, 50);
	printf("\n%s Chan0_value = %s\n\n",attr_name, raw_data_pointer);

	int random_value;
	char random_value_array[10];
	for (int i = 0; i < 5; i++)
	{
		random_value = rand() % 200;
		sprintf(random_value_array,"%ld", random_value);
		printf("Setting chan0 value to %d\n", random_value);

		iio_channel_attr_write(chn, attr_name, random_value_array);

		iio_channel_attr_read(chn, attr_name, raw_data_pointer, 50);
		printf("%s Chan0_value = %s\n",attr_name, raw_data_pointer);
		sleep(1);
	}

	printf("\n");

return 0;
}