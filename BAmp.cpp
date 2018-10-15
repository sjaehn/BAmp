#include <cmath>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <array>
#include "BAmp.h"

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

class Amp {
public:
	Amp ();
	void run (uint32_t n_samples);
	void connect_port (uint32_t portnr, float* data);

private:
	std::array<float*, AMP_N_PORTS> port;
};

Amp::Amp ()
{
	port.fill (NULL);
}

void Amp::connect_port (uint32_t portnr, float* data)
{
	if (portnr < AMP_N_PORTS) port [portnr] = data;
}

void Amp::run (uint32_t n_samples)
{
	const float gain   = *port[AMP_GAIN];
	const float* const input  = port[AMP_INPUT];
	float* const output = port[AMP_OUTPUT];

	const float coef = DB_CO(gain);

	uint32_t pos;
	for (pos = 0; pos < n_samples; ++pos)
	{
		output[pos] = input[pos] * coef;
	}
}


static LV2_Handle
instantiate(const LV2_Descriptor* descriptor, double rate,
	const char* bundle_path, const LV2_Feature* const* features)
{
	Amp* instance = new Amp ();
	return (LV2_Handle) instance;
}

static void
connect_port(LV2_Handle instance, uint32_t port, void* data)
{
	Amp* amp = (Amp*)instance;
	amp->connect_port (port, (float*) data);
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
	Amp* amp = (Amp*) instance;
	amp->run (n_samples);
}

static void
cleanup(LV2_Handle instance)
{
	Amp* amp = (Amp*) instance;
	delete amp;
}

static void
activate(LV2_Handle instance)
{
}

static void
deactivate(LV2_Handle instance)
{
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

static const LV2_Descriptor descriptor = {
	AMP_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	  case 0:  return &descriptor;
	  default: return NULL;
	}
}
