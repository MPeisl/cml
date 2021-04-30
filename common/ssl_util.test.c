#include "munit.h"

#include "macro.h"
#include "file.h"
#include "ssl_util.h"
#include "mem.h"

// Test vectors

static uint8_t cert_valid[] =
	"-----BEGIN CERTIFICATE-----\nMIIFazCCA1OgAwIBAgIUYKzktvvkr1SGxBACG6IZBJ8C/DQwDQYJKoZIhvcNAQEL\nBQAwRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3RhdGUxITAfBgNVBAoM\nGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDAeFw0yMTA0MjkxNzA1NTBaFw0yNDAy\nMTcxNzA1NTBaMEUxCzAJBgNVBAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEw\nHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQwggIiMA0GCSqGSIb3DQEB\nAQUAA4ICDwAwggIKAoICAQDvrScjtPYYsHAEnlG4TO3AUyfs9Irsxta4wyZD4DtD\nz26d2pp7QMOaM9gP0b8Qs5G6QZSrWFx3nQSVeTGGfm/nz9InbqOBFjYW3KTRDl6E\nhsuoyWjpNjh2DrzWM2eEZYona8B/hcmfXfF1v1EhOwZgZ90Lf0JbPkp533sK4lm8\nd9q2tbCZ/THZACeZqfpdvDDsiRJtSaNy3rTy1dMQTiU05sp0QYfhhU5xPw9N8uh3\nU8fNrXk5pQDkQFcCZwJv1yGjlzj9+vC/LcjTJh02xstkMigyblmkmPItMYQrU+28\np3FfWlpfC8rkCSzcJ/KPSMLCaJwkfiu73NnLf10RHOmOC8ztdqD5NVoVwvRBCLVF\nyMDUDB9r0AA/4IZCqqxIQX2mcPOeeeIZR8aKcsYGocpObwE9cnhxQLtbZOHOd6YX\nVGGQELAvRZCJd+G4IVLhAXl47By0I8uYcT45Cn6u6u+EbBh5z+NGZkPKW2PjRfdb\nuQSHV0CfrFrdCp2wNGHQ55EWmSg6weCNQk4Ha6Z+etJ6T+DOOehbvuzykXGvGA99\nTZqNbWGyaR62Ts2gOgmPFVzA5Sobk7F7e9QDAar3C+OvE/gFA5s4ZE7/tM08Bd5j\ntvmSYTDocCNhjymVD/TG9FdALpJgn8K8NBajDfrK15M7xOwdRUU8GH+wxbAR1EeC\njQIDAQABo1MwUTAdBgNVHQ4EFgQUmWFjsGCAiXw1FoIyAFDt3rpF8d0wHwYDVR0j\nBBgwFoAUmWFjsGCAiXw1FoIyAFDt3rpF8d0wDwYDVR0TAQH/BAUwAwEB/zANBgkq\nhkiG9w0BAQsFAAOCAgEABD5uHYLOzUWVmSnYrBmc1qOinq83WNQ8f3/aWQBxJAwS\n3h4VrFS/hIORnFIV6CAm1zrVOPgbNjh3fw+q4lY5R63bLANd7YMVHRjXXDsDXJaq\nWvsOyFYJfcmV9gk0B3FqqnqPyh270rKmFet5q/gaoln2FgBlegRNJjYGJomFRSNt\nMnB4uRxWCLJVC/LZ5oi57HNsLMxxRiiR4aghpkBnX6FkF/JW26eLybT1ohi+s57j\n+HaAtarnRX+g0yB+gomMVaMi0o9GBdLFtqRRScTvAevA6Cyl8zfUmkXfhrCu+uUm\nOgI1+VWfnI3+ytGdjiE2kH8+PZdeKulVd4nUT+uzLml3iJFqrBqU+aUGpaWpQiwq\nb3qlg6rZU7fB0bqokK0m32gk9T1SsVpT8FqoJpEWsQPjGdhurGLUraUxhSIYG7BW\nyEses2DDGKSnz+ltMCWFD/F2kShrE3cnKuItVI7x38YTGRR4J+ptYhnmGpaAMGXS\n1YsLuTo4SOtbsJPDx0/l8Ag73nJh5KRgZPZHvhOAtnQ70jyZxHxeVwEbkrxKQt41\ncIg5r5q3GGNtW4zuEVAHtBQS7IvzGsBZWpRkPEjm0+OGNvvvniu9kJTuMqDnJ4h/\nSa5/uqRv5TvD7unKwHT3xxPKGnnbwuGnKs3DcoPgrt5Ks2IQr470RWwIEV4pGtE=\n-----END CERTIFICATE-----";

static const uint8_t sig_valid[] = {
	0x7D, 0x6E, 0x16, 0xA2, 0x30, 0x76, 0xA0, 0x20, 0x20, 0x43, 0x32, 0xF1, 0xE1, 0x61, 0x1C,
	0xD7, 0x86, 0xF4, 0x84, 0x21, 0x37, 0x38, 0x6F, 0xDF, 0xEC, 0x4D, 0x5C, 0x86, 0xA1, 0x2E,
	0x9D, 0x72, 0x5C, 0xAD, 0x25, 0x38, 0x39, 0xA7, 0x7E, 0x0F, 0x15, 0x49, 0x93, 0xB6, 0xA0,
	0x84, 0x50, 0xA0, 0xBC, 0xF8, 0x0B, 0x73, 0x07, 0x6D, 0x82, 0x58, 0x0C, 0x9A, 0x36, 0xC5,
	0xBF, 0x60, 0xD5, 0xD2, 0xAB, 0xC7, 0x9C, 0x77, 0x8D, 0xD2, 0xF5, 0xC9, 0x9A, 0x89, 0x49,
	0xEE, 0xCE, 0xF3, 0xA0, 0x1B, 0x94, 0x08, 0x1A, 0xF2, 0xEF, 0x99, 0x23, 0xD4, 0xA1, 0xC8,
	0x87, 0x23, 0x3F, 0xD7, 0x9A, 0xCC, 0x84, 0xEE, 0xB3, 0x05, 0xA0, 0xC9, 0x55, 0xB5, 0xA1,
	0xBE, 0x1C, 0x06, 0xF7, 0xE8, 0x6C, 0x5D, 0x33, 0x9D, 0x44, 0xBB, 0x70, 0x6E, 0x03, 0xC6,
	0x55, 0xDD, 0x45, 0x9A, 0x92, 0x42, 0x92, 0x63, 0x27, 0x87, 0xBA, 0xCC, 0x87, 0x28, 0x2D,
	0x61, 0x83, 0xFE, 0x69, 0xB7, 0x9F, 0xB4, 0x38, 0xA1, 0x80, 0xBF, 0xA3, 0x75, 0xF2, 0x52,
	0x36, 0xB5, 0xB8, 0xC3, 0x85, 0x6A, 0x8F, 0xD2, 0xDE, 0xEC, 0x42, 0xD8, 0x82, 0x0D, 0x01,
	0x6F, 0xF2, 0x8E, 0xFA, 0x6C, 0x67, 0xE2, 0x99, 0x30, 0xD3, 0x0E, 0x78, 0x84, 0xE1, 0x1D,
	0x27, 0xD7, 0x3D, 0x32, 0x8F, 0x48, 0xEB, 0x4F, 0x91, 0x9B, 0xF8, 0x22, 0x4A, 0xB9, 0xCC,
	0x4B, 0x27, 0x4E, 0x52, 0xF4, 0xA6, 0x0F, 0x0C, 0x2A, 0x12, 0x54, 0x45, 0x82, 0x20, 0xC4,
	0x0A, 0x58, 0x4E, 0x14, 0x66, 0x88, 0x0F, 0xA0, 0x0F, 0xA5, 0x41, 0x92, 0xD6, 0xEA, 0x58,
	0x37, 0xC5, 0xE3, 0x6F, 0xC8, 0x0E, 0x65, 0x18, 0xDD, 0x7E, 0x66, 0xB1, 0x16, 0x64, 0xA6,
	0xC9, 0xF3, 0xEB, 0xA3, 0x0A, 0x10, 0xFC, 0xD4, 0x89, 0x8F, 0xD0, 0x0A, 0xF0, 0x86, 0x0C,
	0x59, 0xF6, 0xB0, 0x11, 0x11, 0xC9, 0xAF, 0x85, 0xDE, 0x2E, 0x5C, 0x2D, 0x2C, 0xD6, 0x0E,
	0xBE, 0xF7, 0xF5, 0x9C, 0xB8, 0xDA, 0x82, 0xEE, 0x65, 0xDA, 0xE6, 0x2C, 0x73, 0xA0, 0xB4,
	0x0E, 0x09, 0x98, 0x58, 0x65, 0xFE, 0x55, 0xF9, 0x98, 0x11, 0x35, 0x56, 0xD2, 0xC9, 0x8B,
	0x6E, 0x4A, 0x30, 0x44, 0x25, 0xB3, 0x4C, 0x47, 0x37, 0x4A, 0xB1, 0xC7, 0xEF, 0x50, 0x7A,
	0xFF, 0x1B, 0xC1, 0x02, 0x24, 0x1E, 0x56, 0xE7, 0xF6, 0x92, 0x60, 0x3F, 0x1C, 0xE1, 0x33,
	0x17, 0xC0, 0x0B, 0xB9, 0x29, 0x4A, 0xEE, 0x91, 0x48, 0x87, 0xCD, 0xE0, 0x82, 0x37, 0xB2,
	0xE9, 0x9C, 0x8B, 0x82, 0xD4, 0x9E, 0xAB, 0x93, 0x49, 0xFB, 0xC9, 0x69, 0x1E, 0x79, 0x7E,
	0x0D, 0xC7, 0xE6, 0xCE, 0xFB, 0x3B, 0xD5, 0xAE, 0x28, 0x50, 0x31, 0x2B, 0x24, 0xFE, 0xB3,
	0x85, 0x52, 0xB7, 0xF9, 0xFE, 0xBA, 0x52, 0x5C, 0x5D, 0xCE, 0x91, 0xF6, 0x05, 0x41, 0xA4,
	0x73, 0x98, 0x28, 0xDB, 0xF9, 0x1E, 0x52, 0x79, 0xEA, 0x57, 0x3F, 0xA6, 0xF7, 0xE5, 0x58,
	0x7D, 0xE4, 0x62, 0x44, 0xFD, 0x61, 0x79, 0x0A, 0x4A, 0x06, 0x53, 0x82, 0xF9, 0x64, 0xB7,
	0x0E, 0xE9, 0xF3, 0xCD, 0x66, 0x8C, 0x46, 0x71, 0x71, 0xB6, 0x90, 0xF5, 0xB6, 0xEB, 0xB6,
	0x48, 0xA6, 0xEB, 0x88, 0xF2, 0x8A, 0xC5, 0x17, 0xCD, 0x18, 0x8B, 0x80, 0xDB, 0xBC, 0xB0,
	0x27, 0xD1, 0x65, 0xCD, 0x7C, 0x3B, 0x91, 0xCE, 0xEA, 0x40, 0x3E, 0x80, 0x26, 0x3E, 0x2A,
	0x34, 0x7C, 0x79, 0xD5, 0x98, 0xD8, 0xC3, 0x9B, 0x7C, 0x6C, 0xDE, 0xBC, 0x20, 0xF9, 0xE4,
	0x4E, 0x2C, 0x2D, 0x7F, 0xAA, 0x43, 0x23, 0x84, 0xE2, 0x73, 0xC3, 0xCB, 0xBE, 0x83, 0xA2,
	0x51, 0x32, 0xCA, 0x37, 0xB7, 0x78, 0xF9, 0x85, 0x56, 0x2C, 0x03, 0x57, 0xBA, 0xCF, 0x69,
	0x79, 0xF6
};
uint8_t quote_valid[] = { 0x68, 0x61, 0x6c, 0x6c, 0x6f, 0x0a };

static void *
setup(UNUSED const MunitParameter params[], UNUSED void *data)
{
	// Before every test, register a logger so that the logging functionality can run.
	logf_handler_t *logf = logf_register(&logf_test_write, stdout);
	logf_handler_set_prio(logf, LOGF_PRIO_TRACE);

	ssl_init(false, NULL);

	return NULL;
}

static void
tear_down(UNUSED void *fixture)
{
	EVP_cleanup();
	ssl_free();
}

static MunitResult
test_ssl_verify_signature_from_buf(UNUSED const MunitParameter params[], UNUSED void *data)
{
	int ret =
		ssl_verify_signature_from_buf(cert_valid, sizeof(cert_valid), sig_valid,
					      sizeof(sig_valid), quote_valid, sizeof(quote_valid));

	// Check if verification was successful
	munit_assert(ret == 0);

	return MUNIT_OK;
}

static MunitTest tests[] = {
	{
		"ssl_verify_signature_from_buf",    /* name */
		test_ssl_verify_signature_from_buf, /* test */
		setup,				    /* setup */
		tear_down,			    /* tear_down */
		MUNIT_TEST_OPTION_NONE,		    /* options */
		NULL				    /* parameters */
	},

	// Mark the end of the array with an entry where the test function is NULL
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

MunitSuite ssl_util_suite = {
	"test_ssl_utils: ",	/* name */
	tests,			/* tests */
	NULL,			/* suites */
	1,			/* iterations */
	MUNIT_SUITE_OPTION_NONE /* options */
};
