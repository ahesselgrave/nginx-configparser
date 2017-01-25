#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "config_parser.h"


// Test fixture for testing parsing a string literal
class NginxParseConfigTest : public ::testing::Test {
protected:
	bool parse_string(const std::string config_string) {
		std::stringstream config_stream(config_string); 
		return parser_.Parse(&config_stream, &config_);
	}
	bool parse_file(const char* config_file_name) {
		return parser_.Parse(config_file_name, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxParseConfigTest, ValidSimpleStatement) {
	EXPECT_TRUE(parse_string("listen 80;"));
}

TEST_F(NginxParseConfigTest, MissingSemicolonSimpleStatement) {
	EXPECT_FALSE(parse_string("worker_processes 4"));
}

TEST_F(NginxParseConfigTest, CurlyBraceBlock) {
	EXPECT_TRUE(parse_string("server { listen 8080; }"));
}

TEST_F(NginxParseConfigTest, NestedCurlyBraceBlocks) {
	const std::string config_string = \
		"server { \n"
			"\tlocation / { \n"
				"\t\tproxy_pass http://localhost:8080; \n"
			"\t}\n"
		"}";

	EXPECT_TRUE(parse_string(config_string))
		<< config_string;
}

TEST_F(NginxParseConfigTest, EmptyCurlyBlock) {
	EXPECT_TRUE(parse_string("location {}"));
}

TEST_F(NginxParseConfigTest, Comment) {
	EXPECT_TRUE(parse_string("listen 80; # This is for the port"));
}

// Tests the provided example file from the fork
TEST_F(NginxParseConfigTest, ProvidedExampleFile) {
	EXPECT_TRUE(parse_file("resources/example_config"));
}

// Tests the current nginx example config as of 2017-01-18
// See https://www.nginx.com/resources/wiki/start/topics/examples/full/
// 
TEST_F(NginxParseConfigTest, NginxOfficialExampleFile) {
	EXPECT_TRUE(parse_file("resources/nginx_example.conf"));
}
