#pragma once

// Thanks to these guys for solution: https://stackoverflow.com/questions/27529570/simple-zlib-c-string-compression-and-decompressions

#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <cpp-base64/base64.h>

std::string string_compress_encode(const std::string& data)
{
    std::stringstream compressed;
    std::stringstream original;
    original << data;
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(original);
    boost::iostreams::copy(out, compressed);

    /**need to encode here **/
    std::string compressed_encoded = base64_encode(reinterpret_cast<const unsigned char*>(compressed.str().c_str()), compressed.str().length());

    return compressed_encoded;
}

std::string string_decompress_decode(const std::string& data)
{
    std::stringstream compressed_encoded;
    std::stringstream decompressed;
    compressed_encoded << data;

    /** first decode  then decompress **/
    std::string compressed = base64_decode(compressed_encoded.str());

    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(compressed);
    boost::iostreams::copy(in, decompressed);
    return decompressed.str();
}