#include "../../Test.h"
#include <math/numerics/ode/ODESolver.h>

class ODEBDF2TestCase : public Test
{
    bool TestOdeBDF2() {
        auto ode = []([[maybe_unused]] double t, const Matrix<double>& y) {
            return Matrix<double>({ { y(1, 0) }, { -sin(y(0, 0)) } });
        };
        std::vector<double> tInterval = { 0.0, 15.0 };
        Matrix<double> y0             = { { 1.0, 0.5 } };
        ODEOption options             = { 0.10, 1e-18, 500, []([[maybe_unused]] double t, const Matrix<double>& y) {
                                 return Matrix<double>({ { 0.0, 1.0 }, { -cos(y(0, 0)), 0.0 } });
                             } };

        auto foo = ODESolver::odeBDF2(ode, tInterval, y0, options);

        auto yResult = foo.Y;
        auto tResult = foo.T;

        double tExpected[151] = {
            0,
            0.100000000000000,
            0.200000000000000,
            0.300000000000000,
            0.400000000000000,
            0.500000000000000,
            0.600000000000000,
            0.700000000000000,
            0.800000000000000,
            0.900000000000000,
            1.000000000000000,
            1.100000000000000,
            1.200000000000000,
            1.300000000000000,
            1.400000000000000,
            1.500000000000000,
            1.600000000000000,
            1.700000000000000,
            1.800000000000000,
            1.900000000000001,
            2.000000000000000,
            2.100000000000001,
            2.200000000000001,
            2.300000000000001,
            2.400000000000001,
            2.500000000000001,
            2.600000000000001,
            2.700000000000001,
            2.800000000000001,
            2.900000000000001,
            3.000000000000001,
            3.100000000000001,
            3.200000000000002,
            3.300000000000002,
            3.400000000000002,
            3.500000000000002,
            3.600000000000002,
            3.700000000000002,
            3.800000000000002,
            3.900000000000002,
            4.000000000000002,
            4.100000000000001,
            4.200000000000001,
            4.300000000000001,
            4.400000000000000,
            4.500000000000000,
            4.600000000000000,
            4.699999999999999,
            4.799999999999999,
            4.899999999999999,
            4.999999999999998,
            5.099999999999998,
            5.199999999999998,
            5.299999999999997,
            5.399999999999997,
            5.499999999999996,
            5.599999999999996,
            5.699999999999996,
            5.799999999999995,
            5.899999999999995,
            5.999999999999995,
            6.099999999999994,
            6.199999999999994,
            6.299999999999994,
            6.399999999999993,
            6.499999999999993,
            6.599999999999993,
            6.699999999999992,
            6.799999999999992,
            6.899999999999991,
            6.999999999999991,
            7.099999999999991,
            7.199999999999990,
            7.299999999999990,
            7.399999999999990,
            7.499999999999989,
            7.599999999999989,
            7.699999999999989,
            7.799999999999988,
            7.899999999999988,
            7.999999999999988,
            8.099999999999987,
            8.199999999999987,
            8.299999999999986,
            8.399999999999986,
            8.499999999999986,
            8.599999999999985,
            8.699999999999985,
            8.799999999999985,
            8.899999999999984,
            8.999999999999984,
            9.099999999999984,
            9.199999999999983,
            9.299999999999983,
            9.399999999999983,
            9.499999999999982,
            9.599999999999982,
            9.699999999999982,
            9.799999999999981,
            9.899999999999981,
            9.999999999999980,
            10.099999999999980,
            10.199999999999980,
            10.299999999999979,
            10.399999999999979,
            10.499999999999979,
            10.599999999999978,
            10.699999999999978,
            10.799999999999978,
            10.899999999999977,
            10.999999999999977,
            11.099999999999977,
            11.199999999999976,
            11.299999999999976,
            11.399999999999975,
            11.499999999999975,
            11.599999999999975,
            11.699999999999974,
            11.799999999999974,
            11.899999999999974,
            11.999999999999973,
            12.099999999999973,
            12.199999999999973,
            12.299999999999972,
            12.399999999999972,
            12.499999999999972,
            12.599999999999971,
            12.699999999999971,
            12.799999999999971,
            12.899999999999970,
            12.999999999999970,
            13.099999999999969,
            13.199999999999969,
            13.299999999999969,
            13.399999999999968,
            13.499999999999968,
            13.599999999999968,
            13.699999999999967,
            13.799999999999967,
            13.899999999999967,
            13.999999999999966,
            14.099999999999966,
            14.199999999999966,
            14.299999999999965,
            14.399999999999965,
            14.499999999999964,
            14.599999999999964,
            14.699999999999964,
            14.799999999999963,
            14.899999999999963,
            14.999999999999963,
        };
        Matrix<double> yExpected({
        { 1.000000000000000, 0.500000000000000 },   { 1.045733091923598, 0.414661838471952 },
        { 1.082799513366099, 0.327330862019535 },   { 1.111053393650355, 0.238476097051327 },
        { 1.130374889544249, 0.148553036987125 },   { 1.140680999674945, 0.057984172490979 },
        { 1.141927398988589, -0.032834560948831 },  { 1.134108334339758, -0.123517966300675 },
        { 1.117257141395708, -0.213672570916603 },  { 1.091448266533096, -0.302877158218918 },
        { 1.056801008540884, -0.390664495570126 },  { 1.013484881252346, -0.476505619367012 },
        { 0.961726304038299, -0.559798021768018 },  { 0.901816169833998, -0.639859126994274 },
        { 0.834117696335737, -0.715926431452414 },  { 0.759073837047194, -0.787165521836845 },
        { 0.677213430153999, -0.852686806955194 },  { 0.589155217233935, -0.911571159334994 },
        { 0.495608895044187, -0.962903768245897 },  { 0.397372492206873, -1.005814431610981 },
        { 0.295325597226342, -1.039521410521386 },  { 0.190418295792589, -1.063375046603650 },
        { 0.083656073485777, -1.076896827433402 },  { -0.023918644844888, -1.079809663425915 },
        { -0.131247276900189, -1.072055889176200 }, { -0.237276877763360, -1.053800852671048 },
        { -0.340981524005975, -1.025421689323381 }, { -0.441381917964957, -0.987482678171649 },
        { -0.537562056822483, -0.940700113067980 }, { -0.628682145271522, -0.885900632447954 },
        { -0.713987328817316, -0.823977310941714 }, { -0.792812226547064, -0.755847548217251 },
        { -0.864581595153801, -0.682416040452311 }, { -0.928807724597906, -0.604545098627901 },
        { -0.985085334433249, -0.523033500309616 }, { -1.033084810985497, -0.438604099107003 },
        { -1.072544614664889, -0.351899672429645 }, { -1.103263615795186, -0.263485998557484 },
        { -1.125094009435189, -0.173860898948575 }, { -1.137935334511176, -0.083467907939788 },
        { -1.141729994389234, 0.007286727209068 },  { -1.136460552846932, 0.098014922524828 },
        { -1.122148963695872, 0.188326629554382 },  { -1.098857782193480, 0.277809776780588 },
        { -1.066693297376303, 0.366011364745692 },  { -1.025810411311134, 0.452420866891642 },
        { -0.976418969959031, 0.536457189955712 },  { -0.918791119998744, 0.617460542643777 },
        { -0.853269130629904, 0.694690590731172 },  { -0.780272989434593, 0.767332171085467 },
        { -0.700306973873919, 0.834509527433559 },  { -0.613964337555144, 0.895309466978245 },
        { -0.521929258063776, 0.948813010776652 },  { -0.424975293107439, 0.994134076888219 },
        { -0.323959796576941, 1.030462623175779 },  { -0.219814052255867, 1.057108682163625 },
        { -0.113529266047621, 1.073543071518313 },  { -0.006138973071274, 1.079430463603972 },
        { 0.101301195201178, 1.074651059205053 },   { 0.207735138419295, 1.059308306909490 },
        { 0.312127903598085, 1.033721761591263 },   { 0.413485891165370, 0.998405987615325 },
        { 0.510874423624611, 0.954038049052200 },   { 0.603431754669507, 0.901417303377227 },
        { 0.690378982338527, 0.841421759810825 },   { 0.771025735652541, 0.774965161365107 },
        { 0.844771874532960, 0.702958316636211 },   { 0.911105736949274, 0.626277241842623 },
        { 0.969599664048731, 0.545739594410281 },   { 1.019903631291220, 0.462089873140050 },
        { 1.061737823716956, 0.375993050173593 },   { 1.094884937187470, 0.288035739929040 },
        { 1.119182887932607, 0.198733693824473 },   { 1.134518490928193, 0.108544291208113 },
        { 1.140822539386336, 0.017882711894218 },   { 1.138066592711260, -0.072859442416865 },
        { 1.126261662562142, -0.163294218861388 },  { 1.105458875361570, -0.253017157262978 },
        { 1.075752080776188, -0.341587982777875 },  { 1.037282264179467, -0.428513276023907 },
        { 0.990243502346466, -0.513232344511406 },  { 0.934890073703914, -0.595107620473281 },
        { 0.871544199919289, -0.673420963556612 },  { 0.800603762992774, -0.747377184974593 },
        { 0.722549226540826, -0.816115862146656 },  { 0.637948912896294, -0.878732022408228 },
        { 0.547461773748854, -0.934305518988955 },  { 0.451836864847502, -0.981937937783067 },
        { 0.351908910676233, -1.020794768062278 },  { 0.248589624372432, -1.050149523700669 },
        { 0.142854813126739, -1.069425737166388 },  { 0.035727710556051, -1.078232482331863 },
        { -0.071740620799866, -1.076389457485309 }, { -0.178492644007987, -1.063938691342231 },
        { -0.283486083996914, -1.041141483793299 }, { -0.385714630039275, -1.008460990690776 },
        { -0.484226317087865, -0.966532575517047 }, { -0.578138570924160, -0.916125372301482 },
        { -0.666649270810259, -0.858099229110011 }, { -0.749043588851002, -0.793361271180643 },
        { -0.824696749541466, -0.722825820153256 }, { -0.893073170578885, -0.647380512108951 },
        { -0.953722670318087, -0.567860390900950 }, { -1.006274551141200, -0.485030713650674 },
        { -1.050430400386649, -0.399578334566175 }, { -1.085956410370671, -0.312110903533090 },
        { -1.112675929421359, -0.223162735840203 }, { -1.130462838580558, -0.133206042134555 },
        { -1.139236221110243, -0.042666192149279 }, { -1.138956665604015, 0.048060245241842 },
        { -1.129624423777939, 0.138585849860000 },  { -1.111279531251010, 0.228512178773563 },
        { -1.084003890604975, 0.317410147055874 },  { -1.047925205727429, 0.404802069933025 },
        { -1.003222540004976, 0.490146561449057 },  { -0.950133145540140, 0.572827588360286 },
        { -0.888960077395711, 0.652149049842238 },  { -0.820079972718606, 0.727336229434449 },
        { -0.743950252848053, 0.797545274672754 },  { -0.661114916923568, 0.861881439514528 },
        { -0.572208062200419, 0.919426141224800 },  { -0.477954313630157, 0.969271954938187 },
        { -0.379165491863213, 1.010563583652848 },  { -0.276733100579385, 1.042541760422705 },
        { -0.171616558272719, 1.064586178180839 },  { -0.064827502542995, 1.076253124412541 },
        { 0.042589095480701, 1.077303691706821 },   { 0.149575914451049, 1.067719294436736 },
        { 0.255085030736081, 1.047702649423728 },   { 0.358099011929569, 1.017664136477168 },
        { 0.457650020347344, 0.978195220299194 },   { 0.552835827540344, 0.930032065806117 },
        { 0.642831988361338, 0.874013376349908 },   { 0.726899823905234, 0.811036729053466 },
        { 0.804390257130644, 0.742017320661682 },   { 0.874743883586752, 0.667852230714563 },
        { 0.937487910438543, 0.589392270496326 },   { 0.992230747701377, 0.507422424683546 },
        { 1.038655091294414, 0.422650967581400 },   { 1.076510315050661, 0.335706638378509 },
        { 1.105604910595273, 0.247142814387953 },   { 1.125799602534177, 0.157447401360496 },
        { 1.137001640469883, 0.067057109341066 },   { 1.139160642071543, -0.023625165653628 },
        { 1.132266239764126, -0.114211042619546 },  { 1.116347669172942, -0.204306547330683 },
        { 1.091475327863198, -0.293492266690232 },  { 1.057764223465025, -0.381304859423884 },
        { 1.015379115888559, -0.467221091656128 },  { 0.964541035479971, -0.550645668246473 },
        { 0.905534727598532, -0.630904216178659 },
        });
//        std::cout << yResult << std::endl;
        AssertEqual(yExpected, yResult);
        for(unsigned long i = 0; i < yResult.rows(); i++) { AssertEqual(tExpected[i], tResult(i, 0)); }
        return true;
    }

public:
    void run() override { TestOdeBDF2(); }
};

int main() {
    ODEBDF2TestCase().run();
    return 0;
}
