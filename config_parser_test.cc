#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "config_parser.h"


// Test fixture for testing parsing a string literal
class NginxParseStringConfigTest : public ::testing::Test {
protected:
	bool parse_string(const std::string config_string) {
		std::stringstream config_stream(config_string); 
		return parser_.Parse(&config_stream, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

TEST_F(NginxParseStringConfigTest, ValidSimpleStatement) {
	EXPECT_TRUE(parse_string("listen 80;"));
}

TEST_F(NginxParseStringConfigTest, MissingSemicolonSimpleStatement) {
	EXPECT_FALSE(parse_string("worker_processes 4"));
}

TEST_F(NginxParseStringConfigTest, CurlyBraceBlock) {
	EXPECT_TRUE(parse_string("server { listen 8080; }"));
}

TEST_F(NginxParseStringConfigTest, NestedCurlyBraceBlocks) {
	const std::string config_string = \
		"server { \n"
			"\tlocation / { \n"
				"\t\tproxy_pass http://localhost:8080; \n"
			"\t}\n"
		"}";

	EXPECT_TRUE(parse_string(config_string))
		<< config_string;
}

TEST_F(NginxParseStringConfigTest, EmptyCurlyBlock) {
	EXPECT_TRUE(parse_string("location {}"));
}

TEST_F(NginxParseStringConfigTest, Comment) {
	EXPECT_TRUE(parse_string("listen 80; # This is for the port"));
}


// Test fixture for testing parsing from a file
class NginxParseFileConfigTest : public ::testing::Test {
protected:
	bool parse_file(const char* config_file_name) {
		return parser_.Parse(config_file_name, &config_);
	}
	NginxConfigParser parser_;
	NginxConfig config_;
};

// Tests the provided example file from the fork
TEST_F(NginxParseFileConfigTest, ProvidedExampleFile) {
	EXPECT_TRUE(parse_file("example_config"));
}

// Tests the current nginx example config as of 2017-01-18
// See https://www.nginx.com/resources/wiki/start/topics/examples/full/
// 
TEST_F(NginxParseFileConfigTest, NginxOfficialExampleFile) {
	EXPECT_TRUE(parse_file("nginx_example.conf"));
}