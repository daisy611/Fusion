use clap::{App, Arg, ArgMatches};
use experiments::{minionn::construct_minionn, mnist::construct_mnist};
use rand::SeedableRng;
use rand_chacha::ChaChaRng;

const RANDOMNESS: [u8; 32] = [
    0x11, 0xe0, 0x8f, 0xbc, 0x89, 0xa7, 0x34, 0x01, 0x45, 0x86, 0x82, 0xb6, 0x51, 0xda, 0xf4, 0x76,
    0x5d, 0xc9, 0x8d, 0xea, 0x23, 0xf2, 0x90, 0x8f, 0x9d, 0x03, 0xf2, 0x77, 0xd3, 0x4a, 0x52, 0xd2,
];

fn get_args() -> ArgMatches<'static> {
    App::new("cg-client")
        .arg(
            Arg::with_name("model")
                .short("m")
                .long("model")
                .takes_value(true)
                .help("MNIST (0), MiniONN(1)")
                .required(true),
        )
        .arg(
            Arg::with_name("ip")
                .short("i")
                .long("ip")
                .takes_value(true)
                .help("Server IP address")
                .required(true),
        )
        .arg(
            Arg::with_name("port")
                .short("p")
                .long("port")
                .takes_value(true)
                .help("Server port (default 8000)")
                .required(false),
        )
        .get_matches()
}

fn main() {
    let vs = tch::nn::VarStore::new(tch::Device::cuda_if_available());
    let mut rng = ChaChaRng::from_seed(RANDOMNESS);
    let args = get_args();

    let ip = args.value_of("ip").unwrap();
    let port = args.value_of("port").unwrap_or("8000");
    let server_addr = format!("{}:{}", ip, port);

    let model = clap::value_t!(args.value_of("model"), usize).unwrap();
    let network = match model {
        0 => construct_mnist(Some(&vs.root()), 1, 0, &mut rng),
        1 => construct_minionn(Some(&vs.root()), 1, 0, &mut rng),
        _ => panic!(),
    };
    let architecture = (&network).into();

    experiments::latency::client::cg(&server_addr, architecture);
}
