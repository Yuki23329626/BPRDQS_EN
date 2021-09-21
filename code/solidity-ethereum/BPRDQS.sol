pragma solidity >0.6.8;
pragma experimental ABIEncoderV2;

import "./BN256G1.sol";
import "./BN256G2.sol";

contract bn256test{
    
    // The prime q in the base field F_q for G1 and G2
    uint256 q = 21888242871839275222246405745257275088696311157297823662689037894645226208583;
    
    struct G1Point {
        uint256 x;
        uint256 y;
    }
    
    // Encoding of field elements is: X[0] * z + X[1]
    struct G2Point {
        uint256[2] x;
        uint256[2] y;
    }
    
    /// @return the generator of G1
    function get_P1() internal pure returns (G1Point memory) {
        return G1Point(1,2);
    }
    
    /// @return the generator of G2
    function get_P2() internal pure returns (G2Point memory) { // RE+IM
        return G2Point(
            [11559732032986387107991004021392285783925812861821192530917403151452391805634,
            10857046999023057135944570762232829481370756359578518086990519993285655852781],

            [4082367875863433681332203403145435568316851327593401208105741076214120093531,
            8495653923123431417604973247489272438418190587263600148770280649306958101930]
        );
    }

    // the parameters used in our scheme
    uint256[12] data_ours = [1344815266627152914463283187279845014755278383291928694753353909171541394836,1837084702305838889412151982805224136594203407900595612401995120507341638253,19839141298644168206850811700572802443008367594493968284253945076485445614608,11617866393225684691713609743008868608607314573997975980766333001837012379216,7383231868876646222195468749902527960069588114973748733017148496746406391488,1094506385187987980671121129049916252537269967468799114882808093616436420006,15283262374429383375304170429218118790700409077486321569696264151497731261777,766905428022367800974453098558968609485389762749412342090187280333532958245,608484566425133779458653715128378710773148367524777651843586825221680898978,1071912538064115488515953769896709270070486935857413703010556424607006350289,1901780301392249165586607880197462042528159087641747558549477366011048942627,7759350841895642906299761855054985662846019555610802689508201383244709645118];
    
    G1Point Tioj;
    G2Point Ci1;
    G1Point Ci2;
    G1Point Tjio;
    G2Point Cj1;
    G1Point Cj2;
    G1Point ICi;
    G1Point ICj;
    
    // the function of equality test uesd in our scheme
    function testOURS()public returns(bool)
    {
        G1Point memory TCi = G1Point(data_ours[0], data_ours[1]);
        G2Point memory Cj1 = G2Point([data_ours[2],data_ours[3]],[data_ours[4],data_ours[5]]);
        G1Point memory TCj = G1Point(data_ours[6], data_ours[7]);
        G2Point memory Ci1 = G2Point([data_ours[8],data_ours[9]],[data_ours[10],data_ours[11]]);
        
        return pairing_check(TCi, Cj1, TCj, Ci1);
    }
    
    // 以下的 functions 可以在 smart contract 上進行定義在橢圓曲線上的運算，參考用
    function g2add(G2Point memory a, G2Point memory b)internal view returns(G2Point memory r)
    {
        (uint256 x_im, uint256 x_re, uint256 y_im, uint256 y_re) = BN256G2.ecTwistAdd(a.x[1], a.x[0], a.y[1], a.y[0], b.x[1], b.x[0], b.y[1], b.y[0]);
        return G2Point([x_re, x_im], [y_re, y_im]);
    }
    
    function g2mul(uint256 s, G2Point memory a)internal view returns(G2Point memory r)
    {
        (uint256 x_im, uint256 x_re, uint256 y_im, uint256 y_re) = BN256G2.ecTwistMul(s, a.x[1], a.x[0], a.y[1], a.y[0]);
        return G2Point([x_re, x_im], [y_re, y_im]);
    }
    
    function g1add(G1Point memory a, G1Point memory b)internal returns(G1Point memory r)
    {
        (uint256 x, uint256 y) = BN256G1.add([a.x, a.y, b.x, b.y]);
        return G1Point(x, y);
    }
    
    function g1mul(uint256 s, G1Point memory a)internal returns(G1Point memory r)
    {
        (uint256 x, uint256 y) = BN256G1.multiply([a.x, a.y, s]);
        return G1Point(x, y);
    }
    
    function g1negate(G1Point memory a) internal view returns(G1Point memory)
    {
        
        return G1Point(a.x, q-a.y);
    }
    
    function g2negate(G2Point memory a) internal view returns(G2Point memory)
    {
        
        return G2Point(
            [a.x[0], a.x[1]],
            [q - a.y[0], q - a.y[1]]);
    }
    
    function pairing_check(G1Point memory P, G2Point memory Q, G1Point memory R, G2Point memory S)internal returns(bool){
        P = g1negate(P);
        return BN256G1.bn256CheckPairing([P.x, P.y, Q.x[0], Q.x[1], Q.y[0], Q.y[1], R.x, R.y, S.x[0], S.x[1], S.y[0], S.y[1]]);
    }
    
}
