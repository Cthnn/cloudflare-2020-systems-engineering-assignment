# Systems Assignment
## How to build this program
In order to compile the source code, ensure that OpenSSL is installed correctly and the C and Header files are in the same directory as the makefile
## Results of profile
It seems that the results of profiling my requests to the cloudflare worker is on average one of the fastest response times. There are probably two reasons that the workers are fasters. Firstly, compared to the other sites I compared the response times to, the cloudflare worker page is quite minimal. The response sizes are one of the lowest compared to all the other sites. There are not many html elements which means that it does not have to return back a lot of information (compared to google.com which has a response size of about 49589 bytes). Additionally even compare to sites with similar response sizes, the cloudflare worker page on average is either the same or better than most other sites. This may be attributed to the way that workers work on cloudflare's edge network on top of the other obvious factor of there being a node on that network which is close in proximity.
## Screenshots
* Cloudflare Worker Request\
![Cloudflare Worker](screenshots/CFWorker.png)
* Cloudflare Worker Links Request\
![Cloudflare Worker Links Page](screenshots/CFWorkerLinks.png)
* Cloudflare Worker Profile\
![Cloudflare Worker Profile](screenshots/CFWorkerProfile.png)
* Cloudflare Worker Links Profile\
![Cloudflare Worker Links Profile](screenshots/LinksProfiles.png)
* Facebook Request Profile\
![Facebook Profile](screenshots/facebook.png)
* Google Request Profile\
![google Profile](screenshots/google.png)
* My Professional Website Request profile\
![Professional Website Profile](screenshots/website.png)
* Twitter Request Profile\
![Twitter Profile](screenshots/twitter.png)
* Youtube Request Profile\
![Youtube Profile](screenshots/youtube.png)
## What is it?

This exercise is a follow-on to the [General Assignment](https://github.com/cloudflare-hiring/cloudflare-2020-general-engineering-assignment), you'll need to complete that first.  In this assignment you'll write a program that makes a request to the endpoints you created in the General Assignment.  This is a systems assignment so we want to see that you're able to use sockets directly rather than using a library that handles the HTTP request.

## Useful Links

- [A Tour of Go](https://tour.golang.org/welcome/1)
- [The Rust Programming Language](https://doc.rust-lang.org/book/index.html)
- [Cloudflare General Assignment](https://github.com/cloudflare-hiring/cloudflare-2020-general-engineering-assignment)

## Requirements

### 1. Use one of the specified languages

Choose from among C/C++/Go/Rust. If you aren't familiar with these languages, you're not alone! Many engineers join Cloudflare without
specific language experience. See the Useful Links section for some quickstart guides.

### 2. Use an off the shelf build tool

Choose something to build your assignment that works with the language you chose (Cargo, Make, CMake etc.).  Include instructions in your readme on how to build and run your program.  Don't check-in binaries, we won't run a pre-compiled binary.

### 3. Do **NOT** use a library to handle the HTTP request

We want to see how familiar you are with systems work.  Although we would normally recommend using a library to handle HTTP requests, for this assignment we want to see how you handle it yourself.

### 4. Create a CLI tool that makes a request to your links page

Your CLI tool should take an argument that is a full URL (--url).  The tool will make an HTTP request to the URL and print the response directly to the console.  Test the CLI tool by specifying the /links URL in your General Assignment and make sure it prints the entire json document with all your links.

Your CLI tool should also allow a --help parameter that describes how to use it.

Feel free to use a library to handle command line argument parsing (getopt etc.).

### 5. Measure how fast it is

Next, add logic to your tool to profile your page.  Add a new argument --profile that takes a positive integer.  Your tool should make that number of requests to your site.  Time the requests and print:

* The number of requests
* The fastest time
* The slowest time
* The mean & median times
* The percentage of requests that succeeded
* Any error codes returned that weren't a success
* The size in bytes of the smallest response
* The size in bytes of the largest response

Include a screenshot of your tool run against your site and another webpage.

Test your tool against your site and some other websites.  Let us know what you find in your readme.  Include outputs for popular sites and your own.  How do we compare?

## Submitting your project

When submitting your project, you should prepare your code for upload to Greenhouse. The preferred method for doing this is to create a "ZIP archive" of your project folder: for more instructions on how to do this on Windows and Mac, see [this guide](https://www.sweetwater.com/sweetcare/articles/how-to-zip-and-unzip-files/).

Please provide the source code only, a compiled binary is not necessary.
